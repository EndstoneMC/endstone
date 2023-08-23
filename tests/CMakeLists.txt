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

foreach (ENDSTONE_TEST_SOURCE ${ENDSTONE_TEST_SOURCES})
    get_filename_component(ENDSTONE_TEST_NAME ${ENDSTONE_TEST_SOURCE} NAME_WE)
    add_executable(${ENDSTONE_TEST_NAME} ${ENDSTONE_TEST_SOURCES})
    target_link_libraries(${ENDSTONE_TEST_NAME} PRIVATE GTest::gtest_main libendstone)
    gtest_discover_tests(${ENDSTONE_TEST_NAME})
endforeach ()

