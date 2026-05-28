cmake_minimum_required(VERSION 3.15)
project(raknet VERSION 4.081 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(RAKNET_BUILD_SHARED "Build RakNet as a shared library." OFF)

file(GLOB RAKNET_SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
file(GLOB RAKNET_HDRS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h")

if(RAKNET_BUILD_SHARED)
    add_library(raknet SHARED ${RAKNET_SRCS} ${RAKNET_HDRS})
    target_compile_definitions(raknet PRIVATE _RAKNET_DLL)
else()
    add_library(raknet STATIC ${RAKNET_SRCS} ${RAKNET_HDRS})
    target_compile_definitions(raknet PRIVATE _RAKNET_LIB)
endif()

set_target_properties(raknet PROPERTIES OUTPUT_NAME raknet)

target_include_directories(raknet
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/raknet>
)

if(WIN32)
    target_compile_definitions(raknet PRIVATE
        WIN32
        _CRT_SECURE_NO_DEPRECATE
        _CRT_NONSTDC_NO_DEPRECATE
        _WINSOCK_DEPRECATED_NO_WARNINGS
    )
    target_link_libraries(raknet PUBLIC ws2_32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(raknet PUBLIC Threads::Threads)
endif()

# Upstream is 2014-era and emits a lot of warnings on modern toolchains.
if(MSVC)
    target_compile_options(raknet PRIVATE /W0)
else()
    target_compile_options(raknet PRIVATE -w)
endif()

install(TARGETS raknet
    EXPORT raknetTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Source/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raknet
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT raknetTargets
    NAMESPACE raknet::
    FILE raknetTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raknet
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/raknetConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/raknetConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raknet
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/raknetConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/raknetConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/raknetConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/raknet
)
