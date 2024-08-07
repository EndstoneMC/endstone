cmake_minimum_required(VERSION 3.15)
project(endstone)

# Check if the project is being used as dependency
if (NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(EndstoneHeaders)
    return()
endif ()

# =======
# options
# =======
option(CODE_COVERAGE "Enable code coverage reporting" false)
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
if (ENDSTONE_DEVTOOLS_ENABLED)
    add_compile_definitions(ENDSTONE_DEVTOOLS)
endif ()

# ========
# packages
# ========
find_package(base64 CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
find_package(concurrentqueue CONFIG REQUIRED)
find_package(cpptrace CONFIG REQUIRED)
#find_package(EnTT CONFIG REQUIRED)
include(FetchContent)
FetchContent_Declare(EnTT URL https://github.com/skypjack/entt/archive/41aab920b083aa424ac1d27666ce287eeaff6ceb.tar.gz)
FetchContent_MakeAvailable(EnTT)
find_package(expected-lite REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(funchook CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
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
target_link_libraries(endstone_core PUBLIC endstone::headers aklomp::base64 boost::boost concurrentqueue::concurrentqueue
        EnTT::EnTT nonstd::expected-lite glm::glm magic_enum::magic_enum Microsoft.GSL::GSL nlohmann_json::nlohmann_json
        pybind11::embed spdlog::spdlog tomlplusplus::tomlplusplus)
if (UNIX)
    target_link_libraries(endstone_core PUBLIC ${CMAKE_DL_LIBS})
    target_compile_definitions(endstone_core PUBLIC ENDSTONE_DISABLE_DEVTOOLS)
endif ()
target_compile_definitions(endstone_core PUBLIC ENDSTONE_VERSION="${ENDSTONE_VERSION}")

include(GNUInstallDirs)
install(TARGETS endstone_core
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ==================
# endstone::devtools
# ==================
if (ENDSTONE_DEVTOOLS_ENABLED)
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
file(GLOB_RECURSE ENDSTONE_RUNTIME_SOURCE_FILES CONFIGURE_DEPENDS "src/endstone_runtime/*.cpp")
add_library(endstone_runtime SHARED ${ENDSTONE_RUNTIME_SOURCE_FILES})
add_library(endstone::runtime ALIAS endstone_runtime)
target_link_libraries(endstone_runtime PRIVATE endstone::core funchook::funchook cpptrace::cpptrace)
if (ENDSTONE_DEVTOOLS_ENABLED)
    target_link_libraries(endstone_runtime PRIVATE endstone::devtools)
endif ()
if (WIN32)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib ws2_32.lib)
    target_link_options(endstone_runtime PRIVATE "/INCREMENTAL:NO")
endif ()
if (UNIX)
    target_link_libraries(endstone_runtime PRIVATE libelf::libelf)
    target_link_options(endstone_runtime PRIVATE "-Wl,--exclude-libs,ALL")
    target_compile_options(endstone_runtime PRIVATE "-fvisibility=hidden" "-fms-extensions")
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

if (WIN32)
    add_library(endstone_runtime_loader SHARED "src/endstone_runtime/loader.cpp")
    target_compile_definitions(endstone_runtime_loader PRIVATE _CRT_SECURE_NO_WARNINGS ENDSTONE_RUNTIME_LOADER)
    install(TARGETS endstone_runtime_loader DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
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
    add_dependencies(endstone_test test_plugin)
    target_link_libraries(endstone_test PRIVATE endstone::core GTest::gtest_main GTest::gmock_main)

    include(GoogleTest)
    gtest_discover_tests(endstone_test)
endif ()
