cmake_minimum_required(VERSION 3.15)
project(endstone_python LANGUAGES CXX)

find_package(pybind11 CONFIG REQUIRED)

file(GLOB_RECURSE ENDSTONE_PYTHON_SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")

pybind11_add_module(endstone_python MODULE ${ENDSTONE_PYTHON_SOURCE_FILES})
target_include_directories(endstone_python PUBLIC include)
target_link_libraries(endstone_python PRIVATE endstone::api)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(endstone_python PRIVATE "stdc++fs")
endif ()
set_target_properties(endstone_python PROPERTIES OUTPUT_NAME "endstone")

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS endstone_python
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})