cmake_minimum_required(VERSION 3.15)
project(endstone_python LANGUAGES CXX)

find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 REQUIRED)

add_library(endstone_python MODULE
        actor.cpp
        ban.cpp
        block.cpp
        boss.cpp
        command.cpp
        damage.cpp
        endstone_python.cpp
        event.cpp
        form.cpp
        inventory.cpp
        lang.cpp
        level.cpp
        network.cpp
        permissions.cpp
        plugin.cpp
        scheduler.cpp
        scoreboard.cpp
        util.cpp
)
target_link_libraries(endstone_python PRIVATE endstone::endstone pybind11::module)
pybind11_extension(endstone_python)
set_target_properties(endstone_python PROPERTIES CXX_VISIBILITY_PRESET "hidden")
target_compile_definitions(endstone_python PUBLIC PYBIND11_DETAILED_ERROR_MESSAGES)
if (NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    target_link_libraries(endstone_python PRIVATE pybind11::lto)
endif ()

if (MSVC)
    target_link_libraries(endstone_python PRIVATE pybind11::windows_extras)
    target_link_options(endstone_python PRIVATE /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF)
    target_compile_options(endstone_python PRIVATE /O2 /DNDEBUG /Zi /Gy)
endif ()
if (UNIX)
    target_link_options(endstone_python PRIVATE -g)
    target_compile_options(endstone_python PRIVATE -O2 -DNDEBUG -g)
    if (ENDSTONE_SEPARATE_DEBUG_INFO)
        add_custom_command(
                TARGET endstone_python
                POST_BUILD
                COMMAND ${CMAKE_OBJCOPY} --only-keep-debug "$<TARGET_FILE:endstone_python>" "$<TARGET_FILE_DIR:endstone_python>/endstone_python.dbg"
                COMMAND ${CMAKE_OBJCOPY} --strip-debug "$<TARGET_FILE:endstone_python>"
                COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink="$<TARGET_FILE_DIR:endstone_python>/endstone_python.dbg" "$<TARGET_FILE:endstone_python>"
                COMMENT "Generating debug symbols and stripping binary: $<TARGET_FILE:endstone_python>"
        )
    endif ()
endif ()

include(GNUInstallDirs)
install(TARGETS endstone_python DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
if (MSVC)
    install(FILES $<TARGET_PDB_FILE:endstone_python> DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
endif ()
if (UNIX AND ENDSTONE_SEPARATE_DEBUG_INFO)
    install(FILES "$<TARGET_FILE_DIR:endstone_python>/endstone_python.dbg" DESTINATION "endstone/_internal/" COMPONENT endstone_wheel OPTIONAL)
endif ()
