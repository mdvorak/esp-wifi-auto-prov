#include <app_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>

static const char TAG[] = "example";

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
    ESP_ERROR_CHECK(app_wifi_print_qr_code_handler_register(NULL));

    // Setup WiFi
    struct app_wifi_config wifi_cfg = {
        .security = WIFI_PROV_SECURITY_1,
    };
    ESP_ERROR_CHECK(app_wifi_init(&wifi_cfg));
    ESP_ERROR_CHECK(app_wifi_start(true));

    // Setup complete
    ESP_LOGI(TAG, "started");
}
