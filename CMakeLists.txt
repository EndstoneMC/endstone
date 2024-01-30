cmake_minimum_required(VERSION 3.15)
project(endstone)


option(CODE_COVERAGE "Enable code coverage reporting" false)
if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()

    if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif ()
endif ()


add_compile_definitions(PYBIND11_DETAILED_ERROR_MESSAGES)


# ========
# packages
# ========
find_package(fmt CONFIG REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(funchook CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
if (UNIX)
    find_package(LIEF CONFIG REQUIRED)
endif ()

find_package(GTest CONFIG REQUIRED)


# =================
# endstone::headers
# =================
add_library(endstone_headers INTERFACE)
add_library(endstone::headers ALIAS endstone_headers)
target_include_directories(endstone_headers INTERFACE include)
target_link_libraries(endstone_headers INTERFACE fmt::fmt)

include(GNUInstallDirs)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# ================
# endstone::python
# ================
file(GLOB_RECURSE ENDSTONE_PYTHON_SOURCE_FILES CONFIGURE_DEPENDS "src/endstone_python/*.cpp")
pybind11_add_module(endstone_python MODULE ${ENDSTONE_PYTHON_SOURCE_FILES})
target_include_directories(endstone_python PUBLIC include)
target_link_libraries(endstone_python PRIVATE endstone::headers)

include(GNUInstallDirs)
install(TARGETS endstone_python DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)


# ==============
# endstone::core
# ==============
file(GLOB_RECURSE ENDSTONE_CORE_SOURCE_FILES CONFIGURE_DEPENDS "src/endstone_core/*.cpp")
add_library(endstone_core ${ENDSTONE_CORE_SOURCE_FILES})
add_library(endstone::core ALIAS endstone_core)
target_link_libraries(endstone_core PUBLIC endstone::headers spdlog::spdlog)
if (UNIX)
    target_link_libraries(endstone_core PUBLIC ${CMAKE_DL_LIBS})
endif ()
target_compile_definitions(endstone_core PUBLIC ENDSTONE_VERSION="${ENDSTONE_VERSION}")

include(GNUInstallDirs)
install(TARGETS endstone_core
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


# =================
# endstone::runtime
# =================
file(GLOB_RECURSE ENDSTONE_RUNTIME_SOURCE_FILES CONFIGURE_DEPENDS "src/endstone_runtime/*.cpp")
add_library(endstone_runtime SHARED ${ENDSTONE_RUNTIME_SOURCE_FILES})
add_library(endstone::runtime ALIAS endstone_runtime)
target_link_libraries(endstone_runtime PRIVATE endstone::core funchook::funchook magic_enum::magic_enum pybind11::embed)
if (WIN32)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib)
elseif (UNIX)
    target_link_libraries(endstone_runtime PRIVATE LIEF::LIEF)
    target_link_options(endstone_runtime PRIVATE "-Wl,--exclude-libs,ALL")
    target_compile_options(endstone_runtime PRIVATE "-fvisibility=hidden")
endif ()

include(GNUInstallDirs)
install(TARGETS endstone_runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS endstone_runtime DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)


# =====
# tests
# =====
if (NOT BUILD_TESTING STREQUAL OFF)
    file(GLOB ENDSTONE_TEST_FILES "tests/*.cpp")
    add_executable(endstone_test ${ENDSTONE_TEST_FILES})
    target_link_libraries(endstone_test PRIVATE GTest::gtest_main endstone::core)
    include(GoogleTest)
    gtest_discover_tests(endstone_test)
endif ()
