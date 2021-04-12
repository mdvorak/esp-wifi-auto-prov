#pragma once

#include <esp_err.h>
#include <stdbool.h>
#include <wifi_provisioning/manager.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t app_wifi_init(const char *hostname);

esp_err_t app_wifi_start(wifi_prov_security_t security, bool force_provisioning);

const char *app_wifi_get_prov_pop();

void app_wifi_set_prov_pop(const char *value);

const char *app_wifi_prov_get_service_name();

void app_wifi_prov_set_service_name(const char *service_name);

#ifdef __cplusplus
}
#endif
