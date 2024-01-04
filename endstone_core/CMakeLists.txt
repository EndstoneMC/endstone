project(endstone_core LANGUAGES CXX)


find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)


file(GLOB_RECURSE ENDSTONE_CORE_SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_library(endstone_core ${ENDSTONE_CORE_SOURCE_FILES})
add_library(endstone::core ALIAS endstone_core)
target_include_directories(endstone_core PUBLIC include)
target_link_libraries(endstone_core PUBLIC endstone::api spdlog::spdlog pybind11::embed PRIVATE bedrock::internals)
if (CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(endstone_core PUBLIC ${CMAKE_DL_LIBS} "stdc++fs")
endif ()
target_compile_definitions(endstone_core PUBLIC PYBIND11_USE_SMART_HOLDER_AS_DEFAULT)
if (NOT DEFINED ENDSTONE_VERSION)
    message(FATAL_ERROR "ENDSTONE_VERSION is not defined")
else ()
    target_compile_definitions(endstone_core PUBLIC ENDSTONE_VERSION="${ENDSTONE_VERSION}")
endif ()


include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS endstone_core
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


if (NOT BUILD_TESTING STREQUAL OFF)
    file(GLOB ENDSTONE_CORE_TEST_FILES "test/*.cpp")
    add_executable(endstone_core_test ${ENDSTONE_CORE_TEST_FILES})
    target_link_libraries(endstone_core_test PRIVATE GTest::gtest_main endstone_core)
    include(GoogleTest)
    gtest_discover_tests(endstone_core_test)
endif ()