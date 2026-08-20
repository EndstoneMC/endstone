cmake_minimum_required(VERSION 3.15)
project(endstone_python LANGUAGES CXX)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_python MODULE
        actor.cpp
        attribute.cpp
        ban.cpp
        block.cpp
        boss.cpp
        command.cpp
        damage.cpp
        enchantments.cpp
        endstone_python.cpp
        event.cpp
        form.cpp
        game_rule.cpp
        inventory.cpp
        lang.cpp
        level.cpp
        map.cpp
        nbt.cpp
        permissions.cpp
        plugin.cpp
        potion.cpp
        scheduler.cpp
        scoreboard.cpp
        util.cpp
)
target_link_libraries(_python PRIVATE endstone::endstone pybind11::module)
target_include_directories(_python PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_definitions(_python PUBLIC PYBIND11_DETAILED_ERROR_MESSAGES)

if (WIN32)
    find_package(detours CONFIG REQUIRED)
    pybind11_add_module(_detours MODULE _detours.cpp)
    target_link_libraries(_detours PRIVATE detours::detours)
endif ()

if (MSVC)
    target_link_options(_python PRIVATE /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF)
    target_compile_options(_python PRIVATE /O2 /DNDEBUG /Zi /Gy)
endif ()
if (UNIX)
    target_link_options(_python PRIVATE -g)
    target_compile_options(_python PRIVATE -O2 -DNDEBUG -g)
    if (ENDSTONE_SEPARATE_DEBUG_INFO)
        add_custom_command(
                TARGET _python
                POST_BUILD
                COMMAND ${CMAKE_OBJCOPY} --only-keep-debug "$<TARGET_FILE:_python>" "$<TARGET_FILE_DIR:_python>/_python.dbg"
                COMMAND ${CMAKE_OBJCOPY} --strip-debug "$<TARGET_FILE:_python>"
                COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink="$<TARGET_FILE_DIR:_python>/_python.dbg" "$<TARGET_FILE:_python>"
                COMMENT "Generating debug symbols and stripping binary: $<TARGET_FILE:_python>"
        )
    endif ()
endif ()

option(ENDSTONE_GENERATE_STUBS "Regenerate endstone/**/__init__.pyi from the built _python extension" ON)
if (ENDSTONE_GENERATE_STUBS)
    set(STUBGEN_DRIVER "${CMAKE_SOURCE_DIR}/scripts/stubgen.py")
    set(STUBGEN_ENGINE "${CMAKE_CURRENT_SOURCE_DIR}/stubgen.py")
    set(STUBGEN_PATTERNS "${CMAKE_CURRENT_SOURCE_DIR}/stubgen.pat")

    # griffe imports the extension, so it must run under the very interpreter
    # pybind11 built against.
    find_program(UV_EXECUTABLE uv)
    if (UV_EXECUTABLE)
        set(STUBGEN_COMMAND "${UV_EXECUTABLE}" run --python "${Python_EXECUTABLE}" --script "${STUBGEN_DRIVER}")
    else ()
        execute_process(COMMAND "${Python_EXECUTABLE}" -c "import griffe, lazy_loader, ruff"
                RESULT_VARIABLE STUBGEN_IMPORT_RESULT ERROR_VARIABLE STUBGEN_IMPORT_ERROR OUTPUT_QUIET)
        if (NOT STUBGEN_IMPORT_RESULT EQUAL 0)
            message(FATAL_ERROR
                "Endstone: stub generation needs 'uv' on PATH, or griffe, lazy-loader and ruff importable from\n"
                "  ${Python_EXECUTABLE}\n${STUBGEN_IMPORT_ERROR}"
                "Install them, or reconfigure with -DENDSTONE_GENERATE_STUBS=OFF."
            )
        endif ()
        set(STUBGEN_COMMAND "${Python_EXECUTABLE}" "${STUBGEN_DRIVER}")
    endif ()

    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stubgen.stamp"
            COMMAND ${STUBGEN_COMMAND}
            --extension "$<TARGET_FILE:_python>"
            --engine "${STUBGEN_ENGINE}"
            --pattern-file "${STUBGEN_PATTERNS}"
            --source-dir "${CMAKE_SOURCE_DIR}"
            --work-dir "${CMAKE_CURRENT_BINARY_DIR}/stubgen"
            COMMAND "${CMAKE_COMMAND}" -E touch "${CMAKE_CURRENT_BINARY_DIR}/stubgen.stamp"
            DEPENDS _python "$<TARGET_FILE:_python>" "${STUBGEN_DRIVER}" "${STUBGEN_ENGINE}" "${STUBGEN_PATTERNS}"
            COMMENT "Generating Python stubs from $<TARGET_FILE:_python>"
            VERBATIM
    )
    add_custom_target(endstone_stubs ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/stubgen.stamp")
endif ()

include(GNUInstallDirs)
install(TARGETS _python DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
if (WIN32)
    install(TARGETS _detours DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
    install(PROGRAMS
            "${SENTRY_NATIVE_BIN_DIR}/crashpad_handler.exe"
            "${SENTRY_NATIVE_BIN_DIR}/crashpad_wer.dll"
            DESTINATION "endstone" COMPONENT endstone_wheel)
else ()
    install(PROGRAMS
            "${SENTRY_NATIVE_BIN_DIR}/crashpad_handler"
            DESTINATION "endstone" COMPONENT endstone_wheel)
endif ()
if (MSVC)
    install(FILES $<TARGET_PDB_FILE:_python> DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
endif ()
if (UNIX AND ENDSTONE_SEPARATE_DEBUG_INFO)
    install(FILES "$<TARGET_FILE_DIR:_python>/_python.dbg" DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
endif ()
