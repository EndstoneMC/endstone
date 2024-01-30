cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)


find_package(endstone CONFIG REQUIRED)


add_executable(test_server src/test_server.cpp src/dummy_bedrock.cpp)
target_link_libraries(test_server endstone::core)

add_library(test_plugin SHARED src/test_plugin.cpp)
target_link_libraries(test_plugin endstone::headers)