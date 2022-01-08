cmake_minimum_required(VERSION 3.13.4)

idf_component_register(
        SRCS src/wifi_auto_prov.c
        INCLUDE_DIRS include
        PRIV_REQUIRES app_update
        REQUIRES wifi_provisioning
)
