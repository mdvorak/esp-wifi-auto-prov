cmake_minimum_required(VERSION 3.16.0)

FILE(GLOB_RECURSE component_sources src/*.*)

set(COMPONENT_SRCS ${component_sources})
set(COMPONENT_ADD_INCLUDEDIRS include)

register_component()
