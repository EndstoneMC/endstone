project(endstone_runtime LANGUAGES CXX)

find_package(funchook CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)


file(GLOB_RECURSE ENDSTONE_RUNTIME_SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_library(endstone_runtime SHARED ${ENDSTONE_RUNTIME_SOURCE_FILES})
add_library(endstone::runtime ALIAS endstone_runtime)
target_include_directories(endstone_runtime PUBLIC include)
target_link_libraries(endstone_runtime PUBLIC endstone::api endstone::core PRIVATE bedrock::internals funchook::funchook magic_enum::magic_enum)
if (WIN32)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib)
endif ()


include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS endstone_runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS endstone_runtime DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)