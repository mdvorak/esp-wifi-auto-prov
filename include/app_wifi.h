#pragma once

// DEPRECATION NOTICE
#pragma message("header app_wifi.h is deprecated, please use auto_wifi_prov.h instead")

#include "auto_wifi_prov.h"

#ifdef __cplusplus
extern "C" {
#endif

// COMPATIBILITY DECLARATIONS
#define app_wifi_config auto_wifi_prov_config
typedef esp_err_t (*app_wifi_connect_fn)();

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_init"))) esp_err_t app_wifi_init(const struct app_wifi_config *config)
{
    return auto_wifi_prov_init(config);
}

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_start"))) esp_err_t app_wifi_start(bool force_provisioning)
{
    return auto_wifi_prov_start(force_provisioning);
}

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_get_prov_pop"))) const char *app_wifi_get_prov_pop()
{
    return auto_wifi_prov_get_prov_pop();
}

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_get_service_name"))) const char *app_wifi_prov_get_service_name()
{
    return auto_wifi_prov_get_service_name();
}

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_print_qrcode_link"))) void app_wifi_print_qrcode_link()
{
    auto_wifi_prov_print_qrcode_link();
}

__attribute__((deprecated("this function has been renamed to auto_wifi_prov_print_qr_code_handler_register"))) esp_err_t app_wifi_print_qr_code_handler_register(esp_event_handler_instance_t *context)
{
    return auto_wifi_prov_print_qr_code_handler_register(context);
}

#ifdef __cplusplus
}
#endif
