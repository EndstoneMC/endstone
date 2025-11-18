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
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # Clang specific settings
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        # GNU specific settings
    else ()
        message(FATAL_ERROR "Endstone: Clang or GCC is required on Linux.")
    endif ()
else ()
    message(FATAL_ERROR "Endstone: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR}) is not supported.")
endif ()

# Optimize settings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2" CACHE STRING "" FORCE)  # 02 Optimization
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/O2)  # MSVC 02 Optimization
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
add_subdirectory(third_party)
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
        message(STATUS "Code coverage is enabled.")
    endif ()

    find_package(GTest REQUIRED)
    include(GoogleTest)
    add_subdirectory(tests)
endif ()
