#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/cdefs.h>
#include <wifi_auto_prov.h>

static const char TAG[] = "example";

void compile_deprecated();

_Noreturn void app_main()
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
    ESP_ERROR_CHECK(wifi_auto_prov_print_qr_code_handler_register(NULL));

    // Setup WiFi
    struct wifi_auto_prov_config wifi_cfg = WIFI_AUTO_PROV_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(wifi_auto_prov_init(&wifi_cfg));
    ESP_ERROR_CHECK(wifi_auto_prov_start(true));

    // Setup complete
    ESP_LOGI(TAG, "started");

    // Run
    while (true)
    {
        vTaskDelay(1);
    }
}
