project(endstone_api LANGUAGES CXX)

find_package(fmt CONFIG REQUIRED)

add_library(endstone_api INTERFACE)
add_library(endstone::api ALIAS endstone_api)
target_include_directories(endstone_api INTERFACE include)
target_link_libraries(endstone_api INTERFACE fmt::fmt-header-only)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
