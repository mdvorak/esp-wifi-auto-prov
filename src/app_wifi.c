#include "app_wifi.h"
#include "app_wifi_defs.h"
#include <esp_log.h>
#include <esp_wifi.h>
#include <wifi_reconnect.h>

#if APP_WIFI_PROV_TYPE_BLE
#include <string.h>
#include <wifi_provisioning/scheme_ble.h>
#elif APP_WIFI_PROV_TYPE_SOFT_AP
#include <wifi_provisioning/scheme_softap.h>
#endif

static const char TAG[] = "app_wifi";

#define POP_LEN 9
#define SERVICE_NAME_LEN 30

static esp_timer_handle_t wifi_prov_timeout_timer = NULL;
static wifi_config_t startup_wifi_config = {};
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
        // On timeout, it needs to be reset manually (probably bug in wifi config stack)
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
        wifi_config_t current_cfg = {};
        ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_STA, &current_cfg));

        if (current_cfg.sta.ssid[0] == '\0')
        {
            ESP_LOGI(TAG, "wifi credentials not found after provisioning, trying startup wifi config");
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &startup_wifi_config));
        }
        else if (startup_wifi_config.sta.ssid[0] == '\0')
        {
            // Nothing we can do, no internet connectivity
            ESP_LOGI(TAG, "no wifi credentials found");
        }

        wifi_reconnect_resume();
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

    service_name = calloc(SERVICE_NAME_LEN, sizeof(char));
    if (!service_name)
        return ESP_ERR_NO_MEM;

    snprintf(service_name, SERVICE_NAME_LEN, "PROV_%x", esp_random());
    return ESP_OK;
}

esp_err_t app_wifi_init(const char *hostname)
{
    // Initialize WiFi
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_MAX_MODEM));

    // Hostname
    if (hostname != NULL)
    {
        ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, hostname));
    }

    // Store original STA config, so it can be used on provisioning timeout
    // Note: This is needed, since wifi stack is unable to re-read correct config from NVS after provisioning for some reason
    ESP_ERROR_CHECK(esp_wifi_get_config(ESP_IF_WIFI_STA, &startup_wifi_config));

    // Listen for events
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &wifi_prov_event_handler, NULL));

    // Start reconnection task (this does not initiate the connection)
    ESP_ERROR_CHECK(wifi_reconnect_start()); // NOTE this must be called before connect, otherwise it might miss connected event

    return ESP_OK;
}

esp_err_t app_wifi_start(wifi_prov_security_t security, bool force_provisioning)
{
    // Initialize provisioning
    wifi_prov_mgr_config_t wifi_prof_cfg = {
#if APP_WIFI_PROV_TYPE_BLE
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BTDM,
#elif APP_WIFI_PROV_TYPE_SOFT_AP
        .scheme = wifi_prov_scheme_softap,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE,
#endif
    };
    ESP_ERROR_CHECK(wifi_prov_mgr_init(wifi_prof_cfg));

    // Detect provisioning mode
    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    if (!provisioned || force_provisioning)
    {
        // Provisioning mode
        ESP_LOGI(TAG, "provisioning starting, timeout %d s", APP_WIFI_PROV_TIMEOUT_S);

        if (security != WIFI_PROV_SECURITY_0)
        {
            // Proof of possession
            // NOTE this uses atm MAC, which is stable, and QR code can be printed on the device
            ESP_ERROR_CHECK(device_pop_init());
            ESP_LOGI(TAG, "pop: %s", pop);
        }

        // Service name
        ESP_ERROR_CHECK(service_name_init());
        ESP_LOGI(TAG, "service name: %s", service_name);

        // Start
        ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, NULL));

        esp_timer_create_args_t args = {
            .callback = wifi_prov_timeout_handler,
            .name = "wifi_prov_timeout",
        };
        ESP_ERROR_CHECK(esp_timer_create(&args, &wifi_prov_timeout_timer));
        ESP_ERROR_CHECK(esp_timer_start_once(wifi_prov_timeout_timer, APP_WIFI_PROV_TIMEOUT_S * 1000000));
    }
    else
    {
        // Deallocate wifi provisioning
        wifi_prov_mgr_deinit();

        // Connect to existing wifi
        ESP_ERROR_CHECK(esp_wifi_start());
        wifi_reconnect_resume();
    }

    return ESP_OK;
}

const char *app_wifi_get_prov_pop()
{
    return pop;
}

void app_wifi_set_prov_pop(const char *value)
{
    if (pop)
    {
        free(pop);
    }

    pop = value ? strdup(value) : NULL;
}

const char *app_wifi_prov_get_service_name()
{
    return service_name;
}

void app_wifi_prov_set_service_name(const char *name)
{
    if (service_name)
    {
        free(service_name);
    }

    service_name = name ? strdup(name) : NULL;
}
