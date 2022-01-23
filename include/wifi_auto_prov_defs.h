#pragma once

#include "sdkconfig.h"

#ifndef WIFI_AUTO_PROV_TYPE_BLE
#define WIFI_AUTO_PROV_TYPE_BLE CONFIG_WIFI_AUTO_PROV_TYPE_BLE
#endif

#ifndef WIFI_AUTO_PROV_TYPE_SOFT_AP
#define WIFI_AUTO_PROV_TYPE_SOFT_AP CONFIG_WIFI_AUTO_PROV_TYPE_SOFT_AP
#endif

#ifndef WIFI_AUTO_PROV_TIMEOUT_S
#define WIFI_AUTO_PROV_TIMEOUT_S CONFIG_WIFI_AUTO_PROV_TIMEOUT_S
#endif

#ifndef WIFI_AUTO_PROV_TRANSPORT
/**
 * @brief Transport type string. Either "ble" or "softap".
 */
#define WIFI_AUTO_PROV_TRANSPORT CONFIG_WIFI_AUTO_PROV_TRANSPORT
#endif

#ifndef WIFI_AUTO_PROV_QRCODE_URL
#define WIFI_AUTO_PROV_QRCODE_URL CONFIG_WIFI_AUTO_PROV_QRCODE_URL
#endif

#ifndef WIFI_AUTO_PROV_SERVICE_PREFIX
#define WIFI_AUTO_PROV_SERVICE_PREFIX CONFIG_WIFI_AUTO_PROV_SERVICE_PREFIX
#endif

#if CONFIG_APP_WIFI_PROV_TYPE_BLE
/**
 * @brief Maximum length of service name, including terminating NULL character.
 */
#define WIFI_AUTO_PROV_SERVICE_NAME_LEN 29
#else
/**
 * @brief Maximum length of service name, including terminating NULL character.
 */
#define WIFI_AUTO_PROV_SERVICE_NAME_LEN 33
#endif
