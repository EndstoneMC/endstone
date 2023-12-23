cmake_minimum_required(VERSION 3.24)
project(endstone)


option(CODE_COVERAGE "Enable code coverage reporting" false)
if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
        list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
        include(CodeCoverage)
        append_coverage_compiler_flags()
    endif ()
endif ()


include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(pybind11 URL https://github.com/pybind/pybind11/archive/b6444460eeddc2965ab1a49c6c50c83073779489.zip OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(pybind11)


add_subdirectory(endstone_api)
add_subdirectory(endstone_python)
add_subdirectory(endstone_core)
