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
    get_target_property(TARGET_BINARY_DIR ${ENDSTONE_TEST_NAME} BINARY_DIR)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/lib" ENDSTONE_SOURCE_PATH_NATIVE)

    if (ENABLE_CODE_COVERAGE AND WIN32)
        add_test(
                NAME ${ENDSTONE_TEST_NAME}
                COMMAND OpenCppCoverage
                --sources "${ENDSTONE_SOURCE_PATH_NATIVE}"
                --export_type "cobertura:${PROJECT_SOURCE_DIR}/coverage/reports/${ENDSTONE_TEST_NAME}.xml"
                -- $<TARGET_FILE:${ENDSTONE_TEST_NAME}>
        )
    else ()
        gtest_discover_tests(${ENDSTONE_TEST_NAME})
    endif ()

endforeach ()

