cmake_minimum_required(VERSION 3.15)
project(endstone VERSION 0.11.6 LANGUAGES CXX)

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
if (WIN32)
    # Keep <Windows.h> from dragging in the min/max macros and the rarely-wanted extras
    target_compile_definitions(endstone INTERFACE NOMINMAX WIN32_LEAN_AND_MEAN)
endif ()
if (UNIX)
    # Force use libc++ for ABI compatibility with BDS
    target_compile_options(endstone INTERFACE -stdlib=libc++)
    target_link_options(endstone INTERFACE -stdlib=libc++)
endif ()

function(endstone_add_plugin target_name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "")

    add_library(${target_name} SHARED ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${target_name} PRIVATE endstone::endstone)
    set_target_properties(${target_name} PROPERTIES
            PREFIX "endstone_"
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON)

    if (UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_libraries(${target_name} PRIVATE -static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()
endfunction()
