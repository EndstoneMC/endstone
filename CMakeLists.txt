cmake_minimum_required(VERSION 3.15)
project(endstone LANGUAGES CXX)

# Endstone requires C++ 20
set(CMAKE_CXX_STANDARD 20)

# Compiler Check
if (WIN32)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "Endstone: MSVC is required on Windows.")
    endif ()
elseif (UNIX)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "Endstone: Clang is required on Linux.")
    endif ()
    # Force use libc++ for ABI compatibility with BDS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC" CACHE STRING "" FORCE)
else ()
    message(FATAL_ERROR "Endstone: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR}) is not supported")
endif ()

# Options
option(CODE_COVERAGE "Enable code coverage reporting" OFF)
option(ENDSTONE_ENABLE_DEVTOOLS "Build Endstone with DevTools enabled." OFF)
option(ENDSTONE_SEPARATE_DEBUG_INFO "Separate debug info into .dbg files on Linux using objcopy" OFF)

# Endstone header-only API
add_subdirectory(include)
if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    return()
endif ()

# Implementations
add_subdirectory(src/endstone/python)
add_subdirectory(src/bedrock)
add_subdirectory(src/endstone/core)
add_subdirectory(src/endstone/runtime)

# Test
if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif ()

    find_package(GTest REQUIRED)
    include(GoogleTest)
    add_subdirectory(tests)
endif ()