# Changelog

## [2.1.0](https://github.com/mdvorak/esp-wifi-auto-prov/compare/v2.0.0...v2.1.0) (2022-02-07)


### Features

* added WIFI_AUTO_PROV_CONFIG_DEFAULT constant ([2669803](https://github.com/mdvorak/esp-wifi-auto-prov/commit/2669803a29ec3687680259c38963e88ccdd99faa))
* added wifi_auto_prov_generate_name function ([1568cd0](https://github.com/mdvorak/esp-wifi-auto-prov/commit/1568cd07a4b6fb43bc72bad678ce5964ad24b150))
* added WIFI_AUTO_PROV_SCHEME_EVENT_HANDLER choice to control what is released ([e6b45ce](https://github.com/mdvorak/esp-wifi-auto-prov/commit/e6b45ce779c97e615bea1da2de9275b3f6523757))
* added wifi_auto_prov_set_service_name and wifi_auto_prov_set_pop ([7750f4c](https://github.com/mdvorak/esp-wifi-auto-prov/commit/7750f4ca8d8d4e9111779c2f3e56ef33eb2d8d4f))
* prepend WIFI_AUTO_PROV_SERVICE_PREFIX to service_name, if not present ([701b488](https://github.com/mdvorak/esp-wifi-auto-prov/commit/701b488b8468ffaa7323becf8747cf799e81091c))


### Bug Fixes

* check config service_name and pop for empty string as well ([c879bb3](https://github.com/mdvorak/esp-wifi-auto-prov/commit/c879bb3350d9d16ab84879cc92f17d14eeefa463))
* handle correctly generated service name buffer length ([7e0e0dd](https://github.com/mdvorak/esp-wifi-auto-prov/commit/7e0e0ddc8f73684256fa269b8225c228314367b1))
* use esp_efuse_mac_get_default instead of esp_wifi_get_mac ([e9816b3](https://github.com/mdvorak/esp-wifi-auto-prov/commit/e9816b3150674e4dd8d2fcbd68fe7eac23a934d9))

## [2.0.0](https://github.com/mdvorak/esp-wifi-auto-prov/compare/v1.1.0...v2.0.0) (2022-01-21)


### ⚠ BREAKING CHANGES

* moved deprecated hostname config to last
* one more library rename, final name is wifi_auto_prov
* remove hostname config param

### Features

* added WIFI_AUTO_PROV_SERVICE_PREFIX Kconfig ([65f4e9a](https://github.com/mdvorak/esp-wifi-auto-prov/commit/65f4e9aa015370987d5367402ae28c5335939593))
* one more library rename, final name is wifi_auto_prov ([f26ec3f](https://github.com/mdvorak/esp-wifi-auto-prov/commit/f26ec3fe611b89053e096ca9c4fa70caf9e83cfb))
* remove hostname config param ([7a48d9a](https://github.com/mdvorak/esp-wifi-auto-prov/commit/7a48d9aa54f0db98753071f66e15ed571b5c912b))


### Bug Fixes

* added old compatibility headers ([403f3eb](https://github.com/mdvorak/esp-wifi-auto-prov/commit/403f3eb2f0149fa9b387fbb6106fc5561eeb45a0))
* include "sdkconfig.h" for IDF<=4.2 ([c9b3255](https://github.com/mdvorak/esp-wifi-auto-prov/commit/c9b32557db602a9d3bc5c8718b65a4be212f3546))
* library was never compatible with IDF 4.1 ([215003b](https://github.com/mdvorak/esp-wifi-auto-prov/commit/215003b59616e2009fa3bb3cd4fd3dd87d8c7a50))
* moved deprecated hostname config to last ([bf9fc18](https://github.com/mdvorak/esp-wifi-auto-prov/commit/bf9fc18cbb324d7f60709ac8c284a3f772755873))
* print qrcode link without pop for WIFI_PROV_SECURITY_0 ([d38257f](https://github.com/mdvorak/esp-wifi-auto-prov/commit/d38257fa396d3939da5bbed724b3fd644f9488f8)), closes [#1](https://github.com/mdvorak/esp-wifi-auto-prov/issues/1)
* removed incorrect warning about no wi-fi after successful provisioning ([ca86b62](https://github.com/mdvorak/esp-wifi-auto-prov/commit/ca86b62c65d2d6a2cf282cb02dcb029490d51def))
* removed targets from component info ([0e2723e](https://github.com/mdvorak/esp-wifi-auto-prov/commit/0e2723e7f08489fabf849fd1f110f0fc39dbdfa7))
* run infinite loop in example to avoid WDT ([bd97a06](https://github.com/mdvorak/esp-wifi-auto-prov/commit/bd97a06b40f83d237be1bfcabae30acf87f0fe32))
