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
add_compile_definitions(ENTT_SPARSE_PAGE=2048)
add_compile_definitions(ENTT_PACKED_PAGE=128)

# ========
# packages
# ========
find_package(fmt CONFIG REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(funchook CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(cpptrace CONFIG REQUIRED)
if (UNIX)
    find_package(libelf CONFIG REQUIRED)
endif ()

find_package(GTest CONFIG REQUIRED)


# =================
# endstone::headers
# =================
add_library(endstone_headers INTERFACE)
add_library(endstone::headers ALIAS endstone_headers)
target_include_directories(endstone_headers INTERFACE include)
target_link_libraries(endstone_headers INTERFACE fmt::fmt EnTT::EnTT)

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
target_link_libraries(endstone_core PUBLIC endstone::headers spdlog::spdlog magic_enum::magic_enum)
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
target_link_libraries(endstone_runtime PRIVATE endstone::core funchook::funchook pybind11::embed cpptrace::cpptrace)
if (WIN32)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib)
elseif (UNIX)
    target_link_libraries(endstone_runtime PRIVATE libelf::libelf)
    target_link_options(endstone_runtime PRIVATE "-Wl,--exclude-libs,ALL")
    target_compile_options(endstone_runtime PRIVATE "-fvisibility=hidden")
endif ()

include(GNUInstallDirs)
install(TARGETS endstone_runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS endstone_runtime DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    if (MSVC)
        install(FILES $<TARGET_PDB_FILE:endstone_runtime> DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
    elseif (UNIX)
        add_custom_command(TARGET endstone_python POST_BUILD COMMAND ${CMAKE_STRIP} --strip-debug --strip-unneeded $<TARGET_FILE:endstone_python>)
    endif ()
endif ()

# =====
# tests
# =====
if (NOT BUILD_TESTING STREQUAL OFF)
    add_library(test_plugin SHARED tests/plugin/test_plugin.cpp)
    target_link_libraries(test_plugin PRIVATE endstone::headers)
    set_target_properties(test_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY "plugins")
    set_target_properties(test_plugin PROPERTIES RUNTIME_OUTPUT_DIRECTORY "plugins")

    file(GLOB_RECURSE ENDSTONE_TEST_FILES CONFIGURE_DEPENDS "tests/*.cpp")
    add_executable(endstone_test ${ENDSTONE_TEST_FILES})
    target_link_libraries(endstone_test PRIVATE endstone::core GTest::gtest_main GTest::gmock_main)

    include(GoogleTest)
    gtest_discover_tests(endstone_test)
endif ()
