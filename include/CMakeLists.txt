cmake_minimum_required(VERSION 3.15)
project(endstone VERSION 0.11.5 LANGUAGES CXX)

include(FetchContent)

find_package(expected-lite QUIET)
if (NOT expected-lite_FOUND)
    FetchContent_Declare(
            expected-lite
            GIT_REPOSITORY https://github.com/martinmoene/expected-lite.git
            GIT_TAG v0.8.0
    )
    FetchContent_MakeAvailable(expected-lite)
endif ()

# A git version (passed by conanfile.py) overrides the project() fallback at build time.
if (NOT DEFINED ENDSTONE_VERSION_FULL)
    set(ENDSTONE_VERSION_FULL "${PROJECT_VERSION}")
endif ()
configure_file(endstone/version.h.in "${CMAKE_CURRENT_BINARY_DIR}/generated/endstone/version.h" @ONLY)

add_library(endstone INTERFACE)
add_library(endstone::endstone ALIAS endstone)
target_include_directories(endstone INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>)
target_link_libraries(endstone INTERFACE nonstd::expected-lite)

function(endstone_add_plugin target_name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "")

    add_library(${target_name} SHARED ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${target_name} PRIVATE endstone::endstone)
    set_target_properties(${target_name} PROPERTIES PREFIX "endstone_")

    if (UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_libraries(${target_name} PRIVATE -static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()
endfunction()
