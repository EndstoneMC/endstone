#include <cstdio>
#include <exception>

#include <gtest/gtest.h>

// This optional test-only DLL exposes no server or production bridge entry points.
// Use a fresh host process per invocation because the embedded JVM is process-owned.
extern "C" __declspec(dllexport) int zernyth_run_java_tests(int argc, char **argv) noexcept
{
    try {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
    catch (const std::exception &error) {
        std::fprintf(stderr, "Native JNI test host failed: %s\n", error.what());
        return 2;
    }
    catch (...) {
        std::fprintf(stderr, "Native JNI test host failed with an unknown exception\n");
        return 2;
    }
}
