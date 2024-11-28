cmake_minimum_required(VERSION 3.15)
project(endstone_python LANGUAGES CXX)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 REQUIRED)

pybind11_add_module(endstone_python MODULE
        actor.cpp
        block.cpp
        boss.cpp
        command.cpp
        endstone_python.cpp
        event.cpp
        form.cpp
        inventory.cpp
        lang.cpp
        level.cpp
        network.cpp
        permissions.cpp
        plugin.cpp
        scheduler.cpp
        scoreboard.cpp
        util.cpp
)
target_include_directories(endstone_python PUBLIC include)
target_link_libraries(endstone_python PRIVATE endstone::headers)

include(GNUInstallDirs)
install(TARGETS endstone_python DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" AND UNIX)
    add_custom_command(TARGET endstone_python POST_BUILD COMMAND ${CMAKE_STRIP} --strip-debug --strip-unneeded $<TARGET_FILE:endstone_python>)
endif ()
