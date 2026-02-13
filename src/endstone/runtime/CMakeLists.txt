cmake_minimum_required(VERSION 3.15)
project(endstone_runtime LANGUAGES CXX)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

add_library(endstone_runtime SHARED
        bedrock_hooks/actor.cpp
        bedrock_hooks/bedrock_event_logger.cpp
        bedrock_hooks/campfire_block_actor.cpp
        bedrock_hooks/console_input_reader.cpp
        bedrock_hooks/certificate.cpp
        bedrock_hooks/dedicated_server.cpp
        bedrock_hooks/fire_block.cpp
        bedrock_hooks/health_attribute_delegate.cpp
        bedrock_hooks/inventory_transaction.cpp
        bedrock_hooks/item.cpp
        bedrock_hooks/item_registry_manager.cpp
        bedrock_hooks/level.cpp
        bedrock_hooks/map_data_manager.cpp
        bedrock_hooks/minecraft_commands.cpp
        bedrock_hooks/mob.cpp
        bedrock_hooks/network_connection.cpp
        bedrock_hooks/network_system.cpp
        bedrock_hooks/packet.cpp
        bedrock_hooks/piston_block_actor.cpp
        bedrock_hooks/player.cpp
        bedrock_hooks/raknet_socket2.cpp
        bedrock_hooks/rak_peer_helper.cpp
        bedrock_hooks/repository_sources.cpp
        bedrock_hooks/resource_pack_repository.cpp
        bedrock_hooks/say_command.cpp
        bedrock_hooks/script_actor_gameplay_handler.cpp
        bedrock_hooks/script_block_gameplay_handler.cpp
        bedrock_hooks/script_item_gameplay_handler.cpp
        bedrock_hooks/script_level_gameplay_handler.cpp
        bedrock_hooks/script_player_gameplay_handler.cpp
        bedrock_hooks/script_scripting_event_handler.cpp
        bedrock_hooks/script_server_network_event_handler.cpp
        bedrock_hooks/server_instance.cpp
        bedrock_hooks/server_network_handler.cpp
        bedrock_hooks/server_player.cpp
        bedrock_hooks/server_player_movement_correction_system.cpp
        bedrock_hooks/leaves_block.cpp
        bedrock_hooks/liquid_block.cpp
        main.cpp
        hook.cpp
        vtable_hook.cpp
        linux.cpp
        windows.cpp
)
add_library(endstone::runtime ALIAS endstone_runtime)
set_target_properties(endstone_runtime PROPERTIES COMPILE_WARNING_AS_ERROR ON)
target_link_libraries(endstone_runtime PRIVATE endstone::core funchook::funchook pybind11::embed)
if (MSVC)
    target_link_libraries(endstone_runtime PRIVATE dbghelp.lib ws2_32.lib)
    target_link_options(endstone_runtime PRIVATE /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF)
    target_compile_options(endstone_runtime PRIVATE /O2 /DNDEBUG /Zi /Gy)
endif ()
if (UNIX)
    find_package(libelf REQUIRED)
    target_link_libraries(endstone_runtime PRIVATE libelf::libelf)
    target_link_options(endstone_runtime PRIVATE -g -Wl,--no-undefined,--exclude-libs,ALL)
    target_compile_options(endstone_runtime PRIVATE -O2 -DNDEBUG -g -fvisibility=hidden -fms-extensions -fno-delete-null-pointer-checks)
    if (ENDSTONE_SEPARATE_DEBUG_INFO)
        add_custom_command(
                TARGET endstone_runtime
                POST_BUILD
                COMMAND ${CMAKE_OBJCOPY} --only-keep-debug "$<TARGET_FILE:endstone_runtime>" "$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg"
                COMMAND ${CMAKE_OBJCOPY} --strip-debug "$<TARGET_FILE:endstone_runtime>"
                COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink="$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg" "$<TARGET_FILE:endstone_runtime>"
                COMMENT "Generating debug symbols and stripping binary: $<TARGET_FILE:endstone_runtime>"
        )
    endif ()
endif ()

if (WIN32)
    add_library(endstone_runtime_loader SHARED loader.cpp)
    target_compile_definitions(endstone_runtime_loader PRIVATE _CRT_SECURE_NO_WARNINGS)
    set_target_properties(endstone_runtime_loader PROPERTIES LINK_FLAGS /EXPORT:DllMain,@1,NONAME)
endif ()

include(GNUInstallDirs)
install(TARGETS endstone_runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
if (WIN32)
    install(TARGETS endstone_runtime RUNTIME DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
    install(TARGETS endstone_runtime_loader RUNTIME DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
    install(FILES $<TARGET_PDB_FILE:endstone_runtime> DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
endif ()
if (UNIX)
    install(TARGETS endstone_runtime LIBRARY DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
    if (ENDSTONE_SEPARATE_DEBUG_INFO)
        install(FILES "$<TARGET_FILE_DIR:endstone_runtime>/endstone_runtime.dbg" DESTINATION "endstone" COMPONENT endstone_wheel OPTIONAL)
    endif ()
endif ()
