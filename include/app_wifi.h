#pragma once

#include <esp_err.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t app_wifi_init(const char *hostname);

esp_err_t app_wifi_start(bool force_provisioning);

const char *app_wifi_get_pop();

#ifdef __cplusplus
}
#endif
