cmake_minimum_required(VERSION 3.15)
project(endstone_core LANGUAGES CXX)

find_package(spdlog CONFIG REQUIRED)

file(GLOB_RECURSE ENDSTONE_CORE_SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_library(endstone_core ${ENDSTONE_CORE_SOURCE_FILES})
add_library(endstone::core ALIAS endstone_core)
target_include_directories(endstone_core PUBLIC include)
target_link_libraries(endstone_core PUBLIC endstone::api PRIVATE spdlog::spdlog)
if (UNIX)
    target_link_libraries(endstone_core PRIVATE ${CMAKE_DL_LIBS})
endif ()

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS endstone_core
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})