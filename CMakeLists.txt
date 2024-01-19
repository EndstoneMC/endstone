cmake_minimum_required(VERSION 3.15)
project(endstone)


option(CODE_COVERAGE "Enable code coverage reporting" false)
if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif ()
endif ()


add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)


add_subdirectory(endstone_api)
add_subdirectory(endstone_python)
add_subdirectory(endstone_core)
add_subdirectory(endstone_runtime)
