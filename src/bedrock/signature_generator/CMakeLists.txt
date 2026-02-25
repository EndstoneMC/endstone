cmake_minimum_required(VERSION 3.15)
project(signature_generator LANGUAGES CXX)

find_package(tomlplusplus CONFIG REQUIRED)

add_executable(signature_generator main.cpp)

target_link_libraries(signature_generator PRIVATE tomlplusplus::tomlplusplus)

add_custom_target(generate_signatures ALL
        COMMAND signature_generator ${CMAKE_CURRENT_SOURCE_DIR}/signatures.toml ${CMAKE_BINARY_DIR}/generated/bedrock_signatures.generated.h
        DEPENDS signature_generator
        COMMENT "Generating bedrock_signatures.generated.h from signatures.toml"
)

add_library(bedrock_signatures INTERFACE)
add_library(bedrock::signatures ALIAS bedrock_signatures)
add_dependencies(bedrock_signatures generate_signatures)
target_include_directories(bedrock_signatures INTERFACE ${CMAKE_BINARY_DIR}/generated)
