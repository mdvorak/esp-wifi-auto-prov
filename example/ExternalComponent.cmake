cmake_minimum_required(VERSION 3.11.0)
include(FetchContent)

function(ExternalComponent_Get name git_repository git_ref)
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${git_repository}
            GIT_TAG ${git_ref}
            SOURCE_DIR ${CMAKE_BINARY_DIR}/_components/${name})
    FetchContent_GetProperties(${name})
    string(TOLOWER ${name} nameLower)
    if (NOT ${nameLower}_POPULATED)
        FetchContent_Populate(${name})
    endif ()

    list(APPEND EXTRA_COMPONENT_DIRS ${CMAKE_BINARY_DIR}/_components)
    list(REMOVE_DUPLICATES EXTRA_COMPONENT_DIRS)
    set(EXTRA_COMPONENT_DIRS ${EXTRA_COMPONENT_DIRS} PARENT_SCOPE)

    set(${nameLower}_POPULATED ${${nameLower}_POPULATED} PARENT_SCOPE)
    set(${nameLower}_SOURCE_DIR ${${nameLower}_SOURCE_DIR} PARENT_SCOPE)
    set(${nameLower}_BINARY_DIR ${${nameLower}_BINARY_DIR} PARENT_SCOPE)
endfunction()

function(ExternalLibrary_Get name git_repository git_ref)
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${git_repository}
            GIT_TAG ${git_ref}
            SOURCE_DIR ${CMAKE_BINARY_DIR}/_libs/${name})
    FetchContent_GetProperties(${name})
    string(TOLOWER ${name} nameLower)
    if (NOT ${nameLower}_POPULATED)
        FetchContent_Populate(${name})
    endif ()

    set(${nameLower}_POPULATED ${${nameLower}_POPULATED} PARENT_SCOPE)
    set(${nameLower}_SOURCE_DIR ${${nameLower}_SOURCE_DIR} PARENT_SCOPE)
    set(${nameLower}_BINARY_DIR ${${nameLower}_BINARY_DIR} PARENT_SCOPE)
endfunction()
