cmake_minimum_required(VERSION 3.15)
project(endstone LANGUAGES CXX)

# Check if the project is being used as dependency
if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(EndstoneHeaders)
    return()
endif ()

# =======
# options
# =======
option(CODE_COVERAGE "Enable code coverage reporting" OFF)
option(ENDSTONE_ENABLE_DEVTOOLS "Build Endstone with DevTools enabled." OFF)

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif ()
endif ()

# ===================
# compile definitions
# ===================
add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)
add_compile_definitions(ENTT_SPARSE_PAGE=2048)
add_compile_definitions(ENTT_PACKED_PAGE=128)
add_compile_definitions(NOMINMAX)

# =================
# endstone::headers
# =================
find_package(fmt REQUIRED)
find_package(expected-lite REQUIRED)

add_library(endstone_headers INTERFACE)
add_library(endstone::headers ALIAS endstone_headers)
target_include_directories(endstone_headers INTERFACE include)
target_link_libraries(endstone_headers INTERFACE fmt::fmt nonstd::expected-lite)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ================
# endstone::python
# ================
add_subdirectory(src/endstone_python)

# ==============
# endstone::core
# ==============
add_subdirectory(src/endstone_core)

# ==================
# endstone::devtools
# ==================
if (ENDSTONE_ENABLE_DEVTOOLS)
    find_package(glew CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(zstr CONFIG REQUIRED)

    file(GLOB_RECURSE ENDSTONE_DEVTOOLS_SOURCE_FILES CONFIGURE_DEPENDS "src/endstone_devtools/*.cpp")
    add_library(endstone_devtools ${ENDSTONE_DEVTOOLS_SOURCE_FILES})
    add_library(endstone::devtools ALIAS endstone_devtools)

    target_include_directories(endstone_devtools PRIVATE "include/endstone/detail/devtools/imgui")
    target_link_libraries(endstone_devtools PUBLIC endstone::core GLEW::GLEW glfw imgui::imgui zstr::zstr)

    include(GNUInstallDirs)
    install(TARGETS endstone_devtools
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif ()

# =================
# endstone::runtime
# =================
add_subdirectory(src/endstone_runtime)

# =====
# tests
# =====
if (NOT BUILD_TESTING STREQUAL OFF)
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
