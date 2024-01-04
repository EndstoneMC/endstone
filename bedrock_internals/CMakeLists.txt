project(bedrock_internals LANGUAGES CXX)

add_library(bedrock_internals INTERFACE)
add_library(bedrock::internals ALIAS bedrock_internals)
target_include_directories(bedrock_internals INTERFACE include)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
