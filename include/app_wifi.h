#pragma once

// DEPRECATION NOTICE
#pragma message("header app_wifi.h is deprecated, please use wifi_auto_prov.h instead")

#include "wifi_auto_prov.h"

#ifdef __cplusplus
extern "C" {
#endif

// COMPATIBILITY DECLARATIONS
#define app_wifi_config wifi_auto_prov_config
typedef esp_err_t (*app_wifi_connect_fn)();

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_init"))) esp_err_t app_wifi_init(const struct app_wifi_config *config)
{
    return wifi_auto_prov_init(config);
}

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_start"))) esp_err_t app_wifi_start(bool force_provisioning)
{
    return wifi_auto_prov_start(force_provisioning);
}

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_get_prov_pop"))) const char *app_wifi_get_prov_pop()
{
    return wifi_auto_prov_get_prov_pop();
}

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_get_service_name"))) const char *app_wifi_prov_get_service_name()
{
    return wifi_auto_prov_get_service_name();
}

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_print_qrcode_link"))) void app_wifi_print_qrcode_link()
{
    wifi_auto_prov_print_qrcode_link();
}

__attribute__((deprecated("this function has been renamed to wifi_auto_prov_print_qr_code_handler_register"))) esp_err_t app_wifi_print_qr_code_handler_register(esp_event_handler_instance_t *context)
{
    return wifi_auto_prov_print_qr_code_handler_register(context);
}

#ifdef __cplusplus
}
#endif
