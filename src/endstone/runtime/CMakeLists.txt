cmake_minimum_required(VERSION 3.15)
project(endstone_runtime LANGUAGES CXX)

find_package(sentry REQUIRED)
include(FetchContent)
FetchContent_Declare(
        funchook
        GIT_REPOSITORY https://github.com/kubo/funchook.git
        GIT_TAG v1.1.3
        GIT_SHALLOW 1
)
set(FUNCHOOK_BUILD_SHARED OFF)
set(FUNCHOOK_BUILD_TESTS OFF)
set(FUNCHOOK_DISASM capstone)
FetchContent_MakeAvailable(funchook)
add_library(funchook::funchook ALIAS funchook-static)

add_library(endstone_runtime SHARED
        linux/hook.cpp
        linux/os.cpp
        windows/hook.cpp
        windows/os.cpp
        crash_handler.cpp
        hook.cpp
        main.cpp
        signal_handler.cpp
)
add_library(endstone::runtime ALIAS endstone_runtime)
target_link_libraries(endstone_runtime PRIVATE endstone::core funchook::funchook sentry::sentry)
if (ENDSTONE_ENABLE_DEVTOOLS)
    target_link_libraries(endstone_runtime PRIVATE endstone::devtools)
endif ()
if (WIN32)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib ws2_32.lib)
    target_link_options(endstone_runtime PRIVATE "/INCREMENTAL:NO")
endif ()
if (UNIX)
    find_package(libelf REQUIRED)
    target_link_libraries(endstone_runtime PRIVATE libelf::libelf)
    target_link_options(endstone_runtime PRIVATE "-Wl,--exclude-libs,ALL")
    target_compile_options(endstone_runtime PRIVATE "-fvisibility=hidden" "-fms-extensions")
    #    add_custom_command(
    #                TARGET endstone_runtime
    #                POST_BUILD
    #                COMMAND ${CMAKE_OBJCOPY} --only-keep-debug "$<TARGET_FILE:endstone_runtime>" "$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg"
    #                COMMAND ${CMAKE_OBJCOPY} --strip-debug "$<TARGET_FILE:endstone_runtime>"
    #                COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink="$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg" "$<TARGET_FILE:endstone_runtime>"
    #                COMMENT "Generating debug symbols and stripping binary: $<TARGET_FILE:endstone_runtime>"
    #        )
endif ()

if (WIN32)
    add_library(endstone_runtime_loader SHARED "runtime_loader.cpp")
    target_compile_definitions(endstone_runtime_loader PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

# The crashpad handler executable must be shipped alongside our shared library
add_custom_target(
        copy_crashpad_handler
        ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${sentry_INCLUDE_DIRS}/../bin/ ${CMAKE_BINARY_DIR}/crashpad
)
add_dependencies(copy_crashpad_handler endstone_runtime)
# Reduce the size of the executable by executing strip if it is present on the system
find_program(STRIP_EXECUTABLE strip)
if (STRIP_EXECUTABLE)
    add_custom_target(
            strip_crashpad_handler
            ALL
            COMMAND ${STRIP_EXECUTABLE} ${CMAKE_BINARY_DIR}/crashpad/crashpad_handler${CMAKE_EXECUTABLE_SUFFIX}
            COMMENT "Stripping crashpad_handler executable for size reduction"
    )
    add_dependencies(strip_crashpad_handler copy_crashpad_handler)
endif ()

include(GNUInstallDirs)
install(TARGETS endstone_runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if (WIN32)
    install(TARGETS endstone_runtime RUNTIME DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
    install(TARGETS endstone_runtime_loader RUNTIME DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
    install(FILES $<TARGET_PDB_FILE:endstone_runtime> DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
endif ()
if (UNIX)
    install(TARGETS endstone_runtime LIBRARY DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
    #    install(FILES "$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg" DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
endif ()
install(DIRECTORY ${CMAKE_BINARY_DIR}/crashpad/ DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)