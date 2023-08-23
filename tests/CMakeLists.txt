FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
#add_executable(
#        hello_test
#        hello_test.cc
#)
#target_link_libraries(
#        hello_test
#        GTest::gtest_main
#)
include(GoogleTest)
gtest_discover_tests(hello_test)