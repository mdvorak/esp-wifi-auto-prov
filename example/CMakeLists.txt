cmake_minimum_required(VERSION 3.15.0)
include(ExternalComponent.cmake)

# In-place use of library in this example
list(APPEND EXTRA_COMPONENT_DIRS " ${CMAKE_SOURCE_DIR}/..")

# NOTE these dependencies are for example only
#ExternalComponent_Get(some_dep https://github.com/mdvorak-iot/some-dep.git v1.0.0)

# Project
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(example)
