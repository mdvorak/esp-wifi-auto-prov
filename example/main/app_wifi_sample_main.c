#include <app_wifi.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <wifi_provisioning/manager.h>

static const char TAG[] = "example";

static void print_pop_handler(__unused void *arg, __unused esp_event_base_t event_base,
                              __unused int32_t event_id, __unused void *event_data)
{
    ESP_LOGI(TAG, "Service name: %s", app_wifi_prov_get_service_name());
    ESP_LOGI(TAG, "Proof of possession: %s", app_wifi_get_prov_pop());
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
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_PROV_EVENT, WIFI_PROV_START, &print_pop_handler, NULL, NULL));

    // Setup WiFi
    ESP_ERROR_CHECK(app_wifi_init(NULL));
    ESP_ERROR_CHECK(app_wifi_start(true));

    // Setup complete
    ESP_LOGI(TAG, "started");
}
