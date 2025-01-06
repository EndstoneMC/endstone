cmake_minimum_required(VERSION 3.15)
project(endstone_test LANGUAGES CXX)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif ()

    find_package(GTest REQUIRED)

    add_library(test_plugin SHARED tests/plugin/test_plugin.cpp)
    target_link_libraries(test_plugin PRIVATE endstone::headers)
    set_target_properties(test_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "plugins")
    set_target_properties(test_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "plugins")

    file(GLOB_RECURSE ENDSTONE_TEST_FILES CONFIGURE_DEPENDS "tests/*.cpp")
    add_executable(endstone_test ${ENDSTONE_TEST_FILES})
    add_dependencies(endstone_test test_plugin)
    target_link_libraries(endstone_test PRIVATE endstone::core GTest::gtest_main GTest::gmock_main)

    include(GoogleTest)
    gtest_discover_tests(endstone_test)
endif ()