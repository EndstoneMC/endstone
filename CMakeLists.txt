cmake_minimum_required(VERSION 3.15)

project(endstone LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Check
if (WIN32)
    if (NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
        message(FATAL_ERROR
            "Endstone: clang-cl is required on Windows, but CMake detected ${CMAKE_CXX_COMPILER_ID} "
            "(frontend variant: ${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}). "
            "Install LLVM/Clang and reconfigure with CC=clang-cl and CXX=clang-cl."
        )
    endif ()
    if (CMAKE_VERSION VERSION_LESS 3.29)
        message(FATAL_ERROR "Endstone: CMake 3.29 or newer is required on Windows to link with lld-link.")
    endif ()
    # Link with the LLVM linker (lld-link) instead of the MSVC linker
    set(CMAKE_LINKER_TYPE LLD)
    # CMake cannot scan for C++20 module imports with clang-cl; Endstone uses no
    # modules, so disable scanning to avoid a hard configure error.
    set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
elseif (UNIX)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR
            "Endstone: Clang is required on Linux, but CMake detected ${CMAKE_CXX_COMPILER_ID}. "
            "Reconfigure with CC=clang and CXX=clang++."
        )
    endif ()
    # Force use libc++ for ABI compatibility with BDS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC" CACHE STRING "" FORCE)
else ()
    message(FATAL_ERROR "Endstone: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR}) is not supported")
endif ()

if (NOT CMAKE_GENERATOR MATCHES "^Ninja")
    message(FATAL_ERROR "Endstone: Ninja is required as the CMake generator. Reconfigure with -G Ninja.")
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
