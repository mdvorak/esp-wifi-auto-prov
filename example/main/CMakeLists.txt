cmake_minimum_required(VERSION 3.13.4)

idf_component_register(
        SRCS
        wifi_auto_prov_example_main.c
        compile_deprecated.c
        INCLUDE_DIRS .
)
