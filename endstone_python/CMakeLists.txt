project(endstone_python LANGUAGES CXX)


find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)


file(GLOB_RECURSE ENDSTONE_PYTHON_SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
pybind11_add_module(endstone_python MODULE ${ENDSTONE_PYTHON_SOURCE_FILES})
target_include_directories(endstone_python PUBLIC include)
target_link_libraries(endstone_python PRIVATE endstone::api)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS endstone_python
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS endstone_python DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
