#include "lib.h"
#include <esp_log.h>

static const char TAG[] = "example";

void app_main()
{
    lib_example();

    // Setup complete
    ESP_LOGI(TAG, "started");
}
