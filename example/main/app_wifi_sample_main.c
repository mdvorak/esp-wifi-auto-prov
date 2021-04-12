#include <app_wifi.h>
#include <esp_log.h>

static const char TAG[] = "example";

void app_main()
{
    ESP_ERROR_CHECK(app_wifi_init(NULL));
    ESP_ERROR_CHECK(app_wifi_start(true));

    ESP_LOGI(TAG, "Proof of possession: %s", app_wifi_get_pop());

    // Setup complete
    ESP_LOGI(TAG, "started");
}
