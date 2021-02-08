set(COMPONENT_REQUIRES esp-lib-template)
set(COMPONENT_PRIV_REQUIRES )

FILE(GLOB_RECURSE srcs *.c *.cpp)
set(COMPONENT_SRCS "${srcs}")
set(COMPONENT_ADD_INCLUDEDIRS .)

register_component()
