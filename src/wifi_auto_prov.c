#include "wifi_auto_prov.h"
#include "wifi_auto_prov_defs.h"
#include <esp_app_format.h>
#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_wifi.h>
#include <string.h>

#if WIFI_AUTO_PROV_TYPE_BLE
#include <wifi_provisioning/scheme_ble.h>
#elif WIFI_AUTO_PROV_TYPE_SOFT_AP
#include <wifi_provisioning/scheme_softap.h>
#endif

static const char TAG[] = "wifi_auto_prov";

#define POP_LEN (9)
#define SERVICE_NAME_LEN (28)
#define SERVICE_NAME_MAX_LEN (32 + 5)

#define HANDLE_ERROR(expr, action)      \
    {                                   \
        esp_err_t err_ = (expr);        \
        if (err_ != ESP_OK) { action; } \
    }                                   \
    (void)0

static esp_timer_handle_t wifi_prov_timeout_timer = NULL;
static wifi_config_t startup_wifi_config = {};
static wifi_prov_security_t security = WIFI_PROV_SECURITY_1;
static wifi_auto_prov_connect_fn wifi_connect = NULL;
static char *pop = NULL;
static char *service_name = NULL;

static void free_prov_params()
{
    free(pop);
    pop = NULL;

    free(service_name);
    service_name = NULL;
}

static void wifi_prov_timeout_timer_delete()
{
    if (wifi_prov_timeout_timer)
    {
        esp_timer_stop(wifi_prov_timeout_timer);
        esp_timer_delete(wifi_prov_timeout_timer);
        wifi_prov_timeout_timer = NULL;
    }
}

static void wifi_prov_timeout_handler(__unused void *arg)
{
    ESP_LOGI(TAG, "provisioning timeout");
    wifi_prov_mgr_stop_provisioning();
    // Note: everything is done in WIFI_PROV_END handler
}

static void wifi_prov_event_handler(__unused void *arg, __unused esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_PROV_START:
        ESP_LOGI(TAG, "provisioning started");
        break;
    case WIFI_PROV_CRED_RECV: {
        wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
        ESP_LOGI(TAG, "provisioning received ssid '%s'", (const char *)wifi_sta_cfg->ssid);
        break;
    }
    case WIFI_PROV_CRED_FAIL: {
        wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
        ESP_LOGE(TAG, "provisioning failed: %s", (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "wifi STA authentication failed" : "wifi AP not found");
        // Note: Let the timeout kill provisioning, even if it won't connect anyway
        break;
    }
    case WIFI_PROV_CRED_SUCCESS:
        ESP_LOGI(TAG, "provisioning successful");
        break;
    case WIFI_PROV_DEINIT:
        // Release memory
        free_prov_params();
        break;
    case WIFI_PROV_END: {
        ESP_LOGI(TAG, "provisioning end");
        wifi_prov_timeout_timer_delete();
        wifi_prov_mgr_deinit();

        // When successful, config should be correctly set
        // On timeout, it needs to be reset manually (probably bug in wi-fi config stack)
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
        wifi_config_t current_cfg = {};
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_get_config(WIFI_IF_STA, &current_cfg));

        if (current_cfg.sta.ssid[0] == '\0')
        {
            ESP_LOGI(TAG, "wifi credentials not found after provisioning, trying previous wifi config");
            ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_config(WIFI_IF_STA, &startup_wifi_config));
        }

        // Connect
        ESP_ERROR_CHECK_WITHOUT_ABORT(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK_WITHOUT_ABORT(wifi_connect());
        break;
    }
    default:
        break;
    }
}

static esp_err_t device_pop_init()
{
    if (pop) return ESP_OK;

    uint8_t eth_mac[6];
    esp_err_t err = esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    if (err != ESP_OK)
        return err;

    pop = calloc(POP_LEN, sizeof(char));
    if (!pop)
        return ESP_ERR_NO_MEM;

    snprintf(pop, POP_LEN, "%02x%02x%02x%02x", eth_mac[2], eth_mac[3], eth_mac[4], eth_mac[5]);
    return ESP_OK;
}

static esp_err_t service_name_init()
{
    if (service_name) return ESP_OK;

    // Allocate
    service_name = calloc(SERVICE_NAME_MAX_LEN, sizeof(char));
    if (!service_name)
        return ESP_ERR_NO_MEM;

    // Read MAC
    uint8_t eth_mac[6];
    esp_err_t err = esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    if (err != ESP_OK)
        return err;

    // Final name
    snprintf(service_name, SERVICE_NAME_MAX_LEN, "%s%02x%02x", WIFI_AUTO_PROV_SERVICE_PREFIX, eth_mac[0], eth_mac[1]);
    // Truncate
    service_name[SERVICE_NAME_LEN] = '\0';
    return ESP_OK;
}

esp_err_t wifi_auto_prov_init(const struct wifi_auto_prov_config *config)
{
    if (config == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = ESP_OK;

    // Initialize WiFi
    HANDLE_ERROR(err = esp_netif_init(), goto exit);
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    HANDLE_ERROR(err = esp_wifi_init(&cfg), goto exit);
    HANDLE_ERROR(err = esp_wifi_set_storage(WIFI_STORAGE_FLASH), goto exit);

    // Hostname
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (config->hostname != NULL)
    {
        HANDLE_ERROR(err = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, config->hostname), goto exit);
    }
#pragma GCC diagnostic pop

    // Copy provision params
    security = config->security;
    wifi_connect = config->wifi_connect ? config->wifi_connect : esp_wifi_connect; // If not set, use esp_wifi_connect()
    if (config->service_name)
    {
        service_name = strdup(config->service_name);
    }
    if (config->pop)
    {
        pop = strdup(config->pop);
    }

    // Store original STA config, so it can be used on provisioning timeout
    // Note: This is needed, since wi-fi stack is unable to re-read correct config from NVS after provisioning for some reason
    HANDLE_ERROR(err = esp_wifi_get_config(WIFI_IF_STA, &startup_wifi_config), goto exit);

    // Listen for events
    HANDLE_ERROR(err = esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &wifi_prov_event_handler, NULL), goto exit);

exit:
    // Done
    return err;
}

esp_err_t wifi_auto_prov_start(bool force_provisioning)
{
    esp_err_t err = ESP_OK;

    // Initialize provisioning
    wifi_prov_mgr_config_t wifi_prof_cfg = {
#if WIFI_AUTO_PROV_TYPE_BLE
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
#elif WIFI_AUTO_PROV_TYPE_SOFT_AP
        .scheme = wifi_prov_scheme_softap,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE,
#endif
    };
    HANDLE_ERROR(err = wifi_prov_mgr_init(wifi_prof_cfg), goto exit);

    // Detect provisioning mode
    bool provisioned = false;
    HANDLE_ERROR(err = wifi_prov_mgr_is_provisioned(&provisioned), goto exit);

    if (!provisioned || force_provisioning)
    {
        // Provisioning mode
        ESP_LOGI(TAG, "provisioning starting, timeout %d s%s", WIFI_AUTO_PROV_TIMEOUT_S, force_provisioning ? " (forced)" : "");

#if WIFI_AUTO_PROV_TYPE_SOFT_AP
        esp_netif_create_default_wifi_ap();
#endif

        if (security != WIFI_PROV_SECURITY_0)
        {
            // Proof of possession
            // NOTE this uses atm MAC, which is stable, and QR code can be printed on the device
            HANDLE_ERROR(err = device_pop_init(), goto exit);
            ESP_LOGI(TAG, "pop: %s", pop);
        }

        // Service name
        HANDLE_ERROR(err = service_name_init(), goto exit);
        ESP_LOGI(TAG, "service name: %s", service_name);

        // Start
        HANDLE_ERROR(err = wifi_prov_mgr_start_provisioning(security, pop, service_name, NULL), goto exit);

        esp_timer_create_args_t args = {
            .callback = wifi_prov_timeout_handler,
            .name = "wifi_prov_timeout",
        };
        HANDLE_ERROR(err = esp_timer_create(&args, &wifi_prov_timeout_timer), goto exit);
        HANDLE_ERROR(err = esp_timer_start_once(wifi_prov_timeout_timer, WIFI_AUTO_PROV_TIMEOUT_S * 1000000), goto exit);
    }
    else
    {
        // Normal operation
        ESP_LOGI(TAG, "already provisioned, connecting to known network");

        // Deallocate wifi provisioning
        wifi_prov_mgr_deinit();

        // Connect to existing wifi
        HANDLE_ERROR(err = esp_wifi_set_mode(WIFI_MODE_STA), goto exit);
        HANDLE_ERROR(err = esp_wifi_start(), goto exit);
        HANDLE_ERROR(wifi_connect(), goto exit);
    }

exit:
    // Done
    return err;
}

const char *wifi_auto_prov_get_prov_pop()
{
    return pop;
}

const char *wifi_auto_prov_get_service_name()
{
    return service_name;
}

void wifi_auto_prov_print_qrcode_link()
{
    const char VER[] = "v1";
    char payload[200] = {};
    if (security != WIFI_PROV_SECURITY_0)
    {
        const char *pop_str = pop != NULL ? pop : "";
        // {"ver":"%s","name":"%s","pop":"%s","transport":"%s"}
        snprintf(payload, sizeof(payload), "%%7B%%22ver%%22%%3A%%22%s%%22%%2C%%22name%%22%%3A%%22%s%%22%%2C%%22pop%%22%%3A%%22%s%%22%%2C%%22transport%%22%%3A%%22%s%%22%%7D",
                 VER, service_name, pop_str, WIFI_AUTO_PROV_TRANSPORT);
    }
    else
    {
        // {"ver":"%s","name":"%s","transport":"%s"}
        snprintf(payload, sizeof(payload), "%%7B%%22ver%%22%%3A%%22%s%%22%%2C%%22name%%22%%3A%%22%s%%22%%2C%%22transport%%22%%3A%%22%s%%22%%7D",
                 VER, service_name, WIFI_AUTO_PROV_TRANSPORT);
    }
    // NOTE print this regardless of log level settings
    printf("PROVISIONING: To view QR Code, copy paste the URL in a browser:\n%s?data=%s\n", WIFI_AUTO_PROV_QRCODE_URL, payload);
}

static void wifi_auto_prov_print_qrcode_link_handler(__unused void *arg, __unused esp_event_base_t event_base,
                                                     __unused int32_t event_id, __unused void *event_data)
{
    wifi_auto_prov_print_qrcode_link();
}

esp_err_t wifi_auto_prov_print_qr_code_handler_register(esp_event_handler_instance_t *context)
{
    return esp_event_handler_instance_register(WIFI_PROV_EVENT, WIFI_PROV_START, wifi_auto_prov_print_qrcode_link_handler, NULL, context);
}
