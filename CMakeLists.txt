cmake_minimum_required(VERSION 3.15.0)

idf_component_register(
        SRCS src/app_wifi.c
        INCLUDE_DIRS include
        REQUIRES wifi_provisioning app_update
)
