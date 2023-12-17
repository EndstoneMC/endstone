cmake_minimum_required(VERSION 3.15)
project(endstone LANGUAGES CXX)

add_subdirectory(endstone_api)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
endif ()