# Changelog

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
