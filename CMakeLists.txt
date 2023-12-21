cmake_minimum_required(VERSION 3.15)
project(endstone LANGUAGES CXX)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
endif ()

add_subdirectory(endstone_api)
add_subdirectory(endstone_core)
add_subdirectory(endstone_python)