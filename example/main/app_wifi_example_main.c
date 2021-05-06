#include <app_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>

static const char TAG[] = "example";

static void print_qrcode_handler(__unused void *arg, __unused esp_event_base_t event_base,
                                 __unused int32_t event_id, __unused void *event_data)
{
    const char VER[] = "v1";
    char payload[200] = {};
    // {"ver":"%s","name":"%s","pop":"%s","transport":"%s"}
    snprintf(payload, sizeof(payload), "%%7B%%22ver%%22%%3A%%22%s%%22%%2C%%22name%%22%%3A%%22%s%%22%%2C%%22pop%%22%%3A%%22%s%%22%%2C%%22transport%%22%%3A%%22%s%%22%%7D",
             VER, app_wifi_prov_get_service_name(), app_wifi_get_prov_pop(), APP_WIFI_PROV_TRANSPORT);
    // NOTE print this regardless of log level settings
    printf("PROVISIONING: To view QR Code, copy paste the URL in a browser:\n%s?data=%s\n", "https://rainmaker.espressif.com/qrcode.html", payload);
}

void app_main()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // System services
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Print PoP on provisioning
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_PROV_EVENT, WIFI_PROV_START, &print_qrcode_handler, NULL, NULL));

    // Setup WiFi
    struct app_wifi_config wifi_cfg = {
        .security = WIFI_PROV_SECURITY_1,
    };
    ESP_ERROR_CHECK(app_wifi_init(&wifi_cfg));
    ESP_ERROR_CHECK(app_wifi_start(true));

    // Setup complete
    ESP_LOGI(TAG, "started");
}
