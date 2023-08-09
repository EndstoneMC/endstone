cmake_minimum_required(VERSION 3.25)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

FetchContent_Declare(
        CMakeExtraUtils
        GIT_REPOSITORY https://github.com/LecrisUT/CMakeExtraUtils
        GIT_TAG v0.3.0
)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG v2.11.1
)
FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.3.0
)
FetchContent_Declare(
        indicators
        GIT_REPOSITORY https://github.com/p-ranav/indicators.git
        GIT_TAG v2.3
)

FetchContent_MakeAvailable(CMakeExtraUtils pybind11 tomlplusplus indicators)

include(DynamicVersion)
dynamic_version(PROJECT_PREFIX endstone_)

project(endstone VERSION ${PROJECT_VERSION})

add_subdirectory(lib)