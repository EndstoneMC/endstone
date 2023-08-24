FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(
        GLOB_RECURSE ENDSTONE_TEST_SOURCES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "*.cpp"
)

include(GoogleTest)

add_executable(endstone_test ${ENDSTONE_TEST_SOURCES})
target_link_libraries(endstone_test PRIVATE GTest::gtest_main endstone::core)
gtest_discover_tests(endstone_test DISCOVERY_TIMEOUT 60)
