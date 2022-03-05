# wifi_auto_prov

[![test](https://github.com/mdvorak/esp-wifi-auto-prov/actions/workflows/test.yml/badge.svg)](https://github.com/mdvorak/esp-wifi-auto-prov/actions/workflows/test.yml)

Wi-Fi setup and provisioning,
using [Unified Provisioning](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/provisioning/provisioning.html)
.

Phone apps:

* [Android BLE Provisioning app](https://play.google.com/store/apps/details?id=com.espressif.provble)
* [iOS BLE Provisioning app](https://apps.apple.com/in/app/esp-ble-provisioning/id1473590141)

## Usage

To reference this library by your project, add it as git submodule, using command

```shell
git submodule add https://github.com/mdvorak/esp-wifi-auto-prov.git components/wifi_auto_prov
```

For code usage see the [example](./example/main/wifi_auto_prov_example_main.c).

### Configuration

Provisioning mode can be selected via menuconfig. Following config values are supported in the `sdkconfig`:

```
# WiFi provisioning mode
CONFIG_WIFI_AUTO_PROV_TYPE_BLE=y
CONFIG_WIFI_AUTO_PROV_TYPE_SOFT_AP=n
# WiFI Provisioning timeout
CONFIG_WIFI_AUTO_PROV_TIMEOUT_S=120
# QR code URL
CONFIG_WIFI_AUTO_PROV_QRCODE_URL="https://espressif.github.io/esp-jumpstart/qrcode.html"
```

These values are aslo provided as defines, see [app_wifi_defs.h](./include/wifi_auto_prov_defs.h).

### BLE provisioning

Note that for BLE provisioning, BT must be still enabled `sdkconfig`. Add following lines to your `sdkconfig.defaults`:

```
# BLE for provisioning
CONFIG_BT_ENABLED=y
CONFIG_BTDM_CTRL_MODE_BLE_ONLY=y
CONFIG_BTDM_CTRL_MODE_BR_EDR_ONLY=n
CONFIG_BTDM_CTRL_MODE_BTDM=n
CONFIG_BT_NIMBLE_ENABLED=y
```

### Proof of Possession

By default, it uses chip MAC address as Proof of Possession, which is stable. Therefore, QR code can be printed and
attached to the device. 

This is not secure, but easy to use. It can be overridden with custom (secure) string during init. 

## Development

Prepare [ESP-IDF development environment](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#get-started-get-prerequisites)
.

Configure example application with

```
cd example/
idf.py menuconfig
```

Flash it via (in the example dir)

```
idf.py build flash monitor
```

As an alternative, you can use [PlatformIO](https://docs.platformio.org/en/latest/core/installation.html) to build and
flash the example project.
