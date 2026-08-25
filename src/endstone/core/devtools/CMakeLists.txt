cmake_minimum_required(VERSION 3.15)
project(endstone_devtools LANGUAGES CXX)

find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(zstr CONFIG REQUIRED)

string(TOUPPER "${CMAKE_BUILD_TYPE}" ENDSTONE_DEVTOOLS_CONFIG)
find_path(IMGUI_BINDINGS_DIR
        NAMES imgui_impl_glfw.cpp
        HINTS "${imgui_PACKAGE_FOLDER_${ENDSTONE_DEVTOOLS_CONFIG}}" "${imgui_PACKAGE_FOLDER_RELEASE}"
        PATH_SUFFIXES res/bindings bindings
        REQUIRED)

add_library(endstone_devtools
        ${IMGUI_BINDINGS_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_BINDINGS_DIR}/imgui_impl_opengl3.cpp
        imgui_json.cpp
        devtools.cpp
        devtools_command.cpp
        vanilla_data.cpp
)
add_library(endstone::devtools ALIAS endstone_devtools)
target_include_directories(endstone_devtools PRIVATE ${IMGUI_BINDINGS_DIR})
target_link_libraries(endstone_devtools PUBLIC endstone::core glfw imgui::imgui zstr::zstr)
