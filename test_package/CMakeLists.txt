cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)


find_package(endstone CONFIG REQUIRED)


add_executable(test_server src/test_server.cpp)
target_link_libraries(test_server endstone::core)
target_compile_definitions(test_server PRIVATE PYBIND11_USE_SMART_HOLDER_AS_DEFAULT)

add_library(test_plugin SHARED src/test_plugin.cpp)
target_link_libraries(test_plugin endstone::api)