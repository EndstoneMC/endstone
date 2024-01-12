cmake_minimum_required(VERSION 3.15)
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


add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)


add_subdirectory(bedrock_internals)
add_subdirectory(endstone_api)
add_subdirectory(endstone_python)
add_subdirectory(endstone_core)
add_subdirectory(endstone_runtime)
