#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "lib.h"

static const char TAG[] = "example";

static void setup()
{
	lib_example();

	// Setup complete
	ESP_LOGI(TAG, "started");
}

static void run()
{
}

extern "C" void app_main()
{
	setup();
	run();
}
