#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char TAG[] = "example";

void setup()
{
	// Setup complete
	ESP_LOGI(TAG, "started");
}

void loop()
{
	vTaskDelay(1000 / portTICK_PERIOD_MS);
}

extern "C" _Noreturn void app_main()
{
	setup();
	for (;;)
	{
		loop();
	}
}
