add_library(api INTERFACE)
add_library(endstone::api ALIAS api)
target_include_directories(api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})