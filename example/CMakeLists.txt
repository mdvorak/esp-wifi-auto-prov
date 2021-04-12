cmake_minimum_required(VERSION 3.15.0)
include(ExternalComponent.cmake)

# In-place use of library in this example
list(APPEND EXTRA_COMPONENT_DIRS " ${CMAKE_CURRENT_LIST_DIR}/..")

# NOTE these dependencies are for example only
ExternalComponent_Get(wifi_reconnect https://github.com/mdvorak-iot/esp-wifi-reconnect.git 1544945)

# Project
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(app_wifi_example)
