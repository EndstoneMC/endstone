cmake_minimum_required(VERSION 3.15)
project(endstone_devtools LANGUAGES CXX)

find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(zstr CONFIG REQUIRED)

add_library(endstone_devtools
        imgui/imgui_impl_glfw.cpp
        imgui/imgui_impl_opengl3.cpp
        imgui/imgui_json.cpp
        devtools.cpp
        devtools_command.cpp
        vanilla_data.cpp
)
add_library(endstone::devtools ALIAS endstone_devtools)

target_link_libraries(endstone_devtools PUBLIC endstone::core GLEW::GLEW glfw imgui::imgui zstr::zstr)

include(GNUInstallDirs)
install(TARGETS endstone_devtools
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})