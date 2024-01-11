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


add_compile_definitions(PYBIND11_USE_SMART_HOLDER_AS_DEFAULT)
add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)
add_compile_definitions(PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF)
# the above line is a temporary fix for error: "pybind11::handle::inc_ref() is being called while the GIL is either not
# held or invalid." when shutting down server on Windows, due to stricter GIL check on pybind11 >= 2.11.0. We were not
# able to determine what causes this issue for now, leave it for the other day...
# See: https://github.com/pybind/pybind11/issues/4748


add_subdirectory(bedrock_internals)
add_subdirectory(endstone_api)
add_subdirectory(endstone_python)
add_subdirectory(endstone_core)
add_subdirectory(endstone_runtime)
