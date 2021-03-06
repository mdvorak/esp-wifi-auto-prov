#pragma once

#include "wifi_auto_prov_defs.h"
#include <esp_err.h>
#include <stdbool.h>
#include <wifi_provisioning/manager.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Callback for wifi_auto_prov_config.wifi_connect.
 *
 * Signature corresponds to the esp_wifi_connect() function.
 */
typedef esp_err_t (*wifi_auto_prov_connect_fn)();

/**
 * @brief Configuration for WiFi init and provisioning.
 */
struct wifi_auto_prov_config
{
    /**
     * @brief Required security mode.
     *
     * When set to WIFI_PROV_SECURITY_0, no proof of possession is used.
     */
    wifi_prov_security_t security;
    /**
     * @brief Custom name of the provisioning service.
     * WIFI_AUTO_PROV_SERVICE_PREFIX will be prepended to this name automatically, if needed.
     *
     * When unset, part of device MAC address is used with PROV_ prefix.
     */
    const char *service_name;
    /**
     * @brief Custom proof of possession string.
     *
     * When unset, part of device MAC address is used.
     */
    const char *pop;
    /**
     * @brief Function to be used to initiate WiFi connection.
     *
     * Default is esp_wifi_connect().
     */
    wifi_auto_prov_connect_fn wifi_connect;
    /**
     * @brief WiFi network hostname.
     *
     * When unset, no hostname is set.
     * Simply wrapper for tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, hostname).
     */
    __attribute__((deprecated("this is gonna be removed in future versions, use tcpip_adapter_set_hostname instead"))) const char *hostname;
};

/**
 * @brief Use this to initialize struct wifi_auto_prov_config.
 */
#define WIFI_AUTO_PROV_CONFIG_DEFAULT()   \
    {                                     \
        .security = WIFI_PROV_SECURITY_1, \
        .service_name = NULL,             \
        .pop = NULL,                      \
        .wifi_connect = NULL,             \
    }

/**
 * @brief Initialize WiFi. WiFi is not started yet by this command.
 *
 * To start the WiFi, call wifi_auto_prov_start().
 *
 * @param config WiFi init config.
 * @return ESP_OK on success, error otherwise.
 */
esp_err_t wifi_auto_prov_init(const struct wifi_auto_prov_config *config);

/**
 * @brief Start WiFi. Either tries to connect to configured WiFi, or starts provisioning.
 *
 * Provisioning is started automatically, when stored WiFi SSID is empty.
 * Otherwise it can be enforced by the argument.
 *
 * Note that this does not start provisioning, when WiFi connection fails for some reason.
 *
 * @param force_provisioning When true, provisioning is started regardless of WiFi config.
 * @return ESP_OK on success, error otherwise.
 */
esp_err_t wifi_auto_prov_start(bool force_provisioning);

/**
 * @brief Returns used proof of possession for a provisioning.
 *
 * Available only during provisioning, otherwise returns NULL.
 * Should be used in WIFI_PROV_START event handler.
 *
 * When using WIFI_PROV_SECURITY_0, it always returns NULL.
 *
 * @return Proof of possession string, if available. NULL otherwise.
 */
const char *wifi_auto_prov_get_prov_pop();

/**
 * @brief Returns used service name for a provisioning.
 *
 * Available only during provisioning, otherwise returns NULL.
 * Should be used in WIFI_PROV_START event handler.
 *
 * @return Service name string, if available. NULL otherwise.
 */
const char *wifi_auto_prov_get_service_name();

/**
 * @brief Prints URL to stdout, pointing to QR code for provisioning.
 *
 * By default, QR code is static for given device, therefore can be printed
 * on the device. It is used by Espressif mobile apps.
 *
 * Should be used in a handler for the WIFI_PROV_START event.
 */
void wifi_auto_prov_print_qrcode_link();

/**
 * @brief Registers a handler for wifi_auto_prov_print_qrcode_link.
 *
 * @param context Identifying an instance of a registered event handler, can be NULL.
 * @return Result of the esp_event_handler_instance_register call.
 */
esp_err_t wifi_auto_prov_print_qr_code_handler_register(esp_event_handler_instance_t *context);

/**
 * @brief Generate service name into given buffer.
 * Name always have suffix of the first bytes of MAC address stored in EFuse.
 *
 * This can be used to generate custom service_name and/or hostname.
 * If the result is larger then prefix+service_name, it will be cut-off during provisioning.
 *
 * Setting include_prefix might be useful when using this value for both service_name and hostname.
 *
 * @param dst Destination buffer. Cannot be NULL.
 * @param dst_len Destination buffer size.
 * @param friendly_name Friendly service name. Can be NULL or empty string.
 * @param include_prefix Flag whether include WIFI_AUTO_PROV_SERVICE_PREFIX in the resulting string.
 * @return ESP_OK on success, error otherwise. On error, dst is not modified.
 */
esp_err_t wifi_auto_prov_generate_name(char *dst, size_t dst_len, const char *friendly_name, bool include_prefix);

/**
 * @brief Custom proof of possession string.
 *
 * @param new_pop Custom proof of posession string.
 * @return ESP_OK on success, error otherwise.
 */
esp_err_t wifi_auto_prov_set_pop(const char *new_pop);

/**
 * @brief Sets service name. Must be called before wifi_auto_prov_start, otherwise it has no effect.
 *
 * WIFI_AUTO_PROV_SERVICE_PREFIX will be prepended to this name automatically, if needed.
 *
 * @param new_name Custom name of the provisioning service.
 *                 Must be shorter than WIFI_AUTO_PROV_SERVICE_NAME_LEN, including the prefix.
 * @return ESP_OK on success, error otherwise.
 */
esp_err_t wifi_auto_prov_set_service_name(const char *new_name);

#ifdef __cplusplus
}
#endif
