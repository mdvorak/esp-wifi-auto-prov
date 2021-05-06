#pragma once

#include "app_wifi_defs.h"
#include <esp_err.h>
#include <stdbool.h>
#include <wifi_provisioning/manager.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef esp_err_t (*app_wifi_connect_fn)();

struct app_wifi_config
{
    wifi_prov_security_t security;
    const char *service_name;
    const char *pop;
    const char *hostname;
    app_wifi_connect_fn wifi_connect;
};

esp_err_t app_wifi_init(const struct app_wifi_config *config);

esp_err_t app_wifi_start(bool force_provisioning);

const char *app_wifi_get_prov_pop();

const char *app_wifi_prov_get_service_name();

#ifdef __cplusplus
}
#endif
