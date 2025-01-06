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
        bedrock/shared_constants.cpp
        bedrock/certificates/certificate.cpp
        bedrock/certificates/extended_certificate.cpp
        bedrock/certificates/web_token.cpp
        bedrock/config/player_capabilities.cpp
        bedrock/core/debug/bedrock_event_logger.cpp
        bedrock/core/file/path.cpp
        bedrock/core/math/color.cpp
        bedrock/core/resource/resource_helper.cpp
        bedrock/core/sem_ver/sem_version.cpp
        bedrock/core/string/string_hash.cpp
        bedrock/core/utility/binary_stream.cpp
        bedrock/deps/json/value.cpp
        bedrock/deps/raknet/get_time.cpp
        bedrock/deps/raknet/raknet_socket2.cpp
        bedrock/deps/raknet/raknet_types.cpp
        bedrock/entity/gamerefs_entity/entity_registry.cpp
        bedrock/entity/gamerefs_entity/stack_result_storage_entity.cpp
        bedrock/entity/gamerefs_entity/weak_storage_entity.cpp
        bedrock/entity/utilities/synched_actor_data_access.cpp
        bedrock/locale/i18n.cpp
        bedrock/locale/localization.cpp
        bedrock/nbt/compound_tag.cpp
        bedrock/nbt/list_tag.cpp
        bedrock/nbt/nbt_io.cpp
        bedrock/nbt/tag.cpp
        bedrock/network/network_identifier.cpp
        bedrock/network/packet.cpp
        bedrock/network/rak_peer_helper.cpp
        bedrock/network/server_network_handler.cpp
        bedrock/network/packet/crafting_data_packet.cpp
        bedrock/network/packet/start_game_packet.cpp
        bedrock/platform/assigned_thread.cpp
        bedrock/platform/uuid.cpp
        bedrock/resources/pack.cpp
        bedrock/resources/pack_error.cpp
        bedrock/resources/pack_manifest.cpp
        bedrock/resources/pack_source.cpp
        bedrock/resources/resource_pack.cpp
        bedrock/resources/resource_pack_manager.cpp
        bedrock/resources/resource_pack_repository.cpp
        bedrock/resources/resource_pack_stack.cpp
        bedrock/scripting/event_handlers/script_level_gameplay_handler.cpp
        bedrock/server/server_instance.cpp
        bedrock/server/server_level.cpp
        bedrock/server/server_player.cpp
        bedrock/server/commands/command.cpp
        bedrock/server/commands/command_origin.cpp
        bedrock/server/commands/command_origin_loader.cpp
        bedrock/server/commands/command_output.cpp
        bedrock/server/commands/command_registry.cpp
        bedrock/server/commands/command_utils.cpp
        bedrock/server/commands/minecraft_commands.cpp
        bedrock/server/commands/standard/say_command.cpp
        bedrock/server/commands/standard/teleport_command.cpp
        bedrock/util/data_io.cpp
        bedrock/util/index_set.cpp
        bedrock/util/string_byte_output.cpp
        bedrock/world/game_session.cpp
        bedrock/world/minecraft.cpp
        bedrock/world/actor/actor.cpp
        bedrock/world/actor/actor_definition_identifier.cpp
        bedrock/world/actor/mob.cpp
        bedrock/world/actor/synched_actor_data.cpp
        bedrock/world/actor/player/abilities.cpp
        bedrock/world/actor/player/player.cpp
        bedrock/world/attribute/attribute_instance.cpp
        bedrock/world/attribute/attribute_map.cpp
        bedrock/world/events/level_event_coordinator.cpp
        bedrock/world/events/player_event_coordinator.cpp
        bedrock/world/events/scripting_event_coordinator.cpp
        bedrock/world/events/server_instance_event_coordinator.cpp
        bedrock/world/gamemode/game_mode.cpp
        bedrock/world/inventory/network/item_stack_net_id_variant.cpp
        bedrock/world/item/item.cpp
        bedrock/world/item/item_descriptor.cpp
        bedrock/world/item/item_stack.cpp
        bedrock/world/item/item_stack_base.cpp
        bedrock/world/item/registry/creative_item_entry.cpp
        bedrock/world/item/registry/creative_item_registry.cpp
        bedrock/world/item/registry/item_registry.cpp
        bedrock/world/item/registry/item_registry_manager.cpp
        bedrock/world/item/registry/item_registry_ref.cpp
        bedrock/world/level/explosion.cpp
        bedrock/world/level/level.cpp
        bedrock/world/level/biome/registry/biome_registry.cpp
        bedrock/world/level/block/block.cpp
        bedrock/world/level/block/block_descriptor.cpp
        bedrock/world/level/block/block_legacy.cpp
        bedrock/world/level/block/actor/furnace_block_actor.cpp
        bedrock/world/level/block/registry/block_type_registry.cpp
        bedrock/world/level/dimension/dimension.cpp
        bedrock/world/scores/objective.cpp
        bedrock/world/scores/scoreboard.cpp
        bedrock/world/scores/scoreboard_id.cpp
        bedrock/world/scores/server_scoreboard.cpp
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