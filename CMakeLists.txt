cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------
# Compile Definitions
# ------------------------
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
add_definitions(-DPYBIND11_DETAILED_ERROR_MESSAGES)
add_definitions(-DMINECRAFT_VERSION="${MINECRAFT_VERSION}")
add_definitions(-DENDSTONE_VERSION="${ENDSTONE_VERSION}")

# ------------------------
# Includes
# ------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(EndstoneVersion)

# ------------------------
# Project
# ------------------------
project(endstone VERSION ${PROJECT_VERSION})

# ------------------------
# Platform Checks
# ------------------------
include(CheckPlatform)

# ------------------------
# Main Project Check
# if endstone is built as a subproject
# ------------------------
set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif ()

# ------------------------
# Options
# ------------------------
option(ENDSTONE_BUILD_TESTING "Build the unit tests." ${MAIN_PROJECT})

# ------------------------
# Targets
# ------------------------
add_subdirectory(include)

if (MAIN_PROJECT)
    add_subdirectory(src)
endif ()

# ------------------------
# Tests
# ------------------------
if (ENDSTONE_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()