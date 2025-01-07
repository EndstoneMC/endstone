cmake_minimum_required(VERSION 3.15)
project(symbol_generator LANGUAGES CXX)

find_package(tomlplusplus CONFIG REQUIRED)

add_executable(symbol_generator main.cpp)

target_link_libraries(symbol_generator PRIVATE tomlplusplus::tomlplusplus)

add_custom_target(generate_symbols ALL
        COMMAND symbol_generator ${CMAKE_CURRENT_SOURCE_DIR}/symbols.toml ${CMAKE_BINARY_DIR}/generated/bedrock_symbols.generated.h
        DEPENDS symbol_generator
        COMMENT "Generating bedrock_symbols.generated.h from symbols.toml"
)

add_library(bedrock_symbols INTERFACE)
add_library(bedrock::symbols ALIAS bedrock_symbols)
add_dependencies(bedrock_symbols generate_symbols)
target_include_directories(bedrock_symbols INTERFACE ${CMAKE_BINARY_DIR}/generated)