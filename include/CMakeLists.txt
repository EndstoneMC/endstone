cmake_minimum_required(VERSION 3.15)
project(endstone LANGUAGES CXX)

include(FetchContent)

find_package(fmt QUIET)
if (NOT fmt_FOUND)
    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG 11.2.0
    )
    FetchContent_MakeAvailable(fmt)
endif ()

find_package(expected-lite QUIET)
if (NOT expected-lite_FOUND)
    FetchContent_Declare(
            expected-lite
            GIT_REPOSITORY https://github.com/martinmoene/expected-lite.git
            GIT_TAG v0.8.0
    )
    FetchContent_MakeAvailable(expected-lite)
endif ()

add_library(endstone INTERFACE)
add_library(endstone::endstone ALIAS endstone)
target_include_directories(endstone INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(endstone INTERFACE fmt::fmt nonstd::expected-lite)

function(endstone_add_plugin target_name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "")

    add_library(${target_name} SHARED ${ARG_UNPARSED_ARGUMENTS})
    target_link_libraries(${target_name} PRIVATE endstone::endstone)
    set_target_properties(${target_name} PROPERTIES PREFIX "endstone_")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_libraries(${target_name} PRIVATE -static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()
endfunction()
