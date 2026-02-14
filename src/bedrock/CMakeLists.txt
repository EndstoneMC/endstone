cmake_minimum_required(VERSION 3.15)
project(bedrock LANGUAGES CXX)

add_subdirectory(symbol_generator)

find_package(base64 REQUIRED)
find_package(Boost REQUIRED)
find_package(expected-lite QUIET)
find_package(fmt REQUIRED)
find_package(EnTT REQUIRED)
find_package(glm REQUIRED)
find_package(magic_enum REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_library(bedrock STATIC
        shared_constants.cpp
        certificates/certificate.cpp
        certificates/web_token.cpp
        config/player_capabilities.cpp
        core/file/path.cpp
        core/math/color.cpp
        core/resource/resource_helper.cpp
        core/sem_ver/sem_version.cpp
        core/string/string_hash.cpp
        core/utility/binary_stream.cpp
        core/utility/serialize_simple_types.cpp
        crypto/encoding/base64.cpp
        deps/json/value.cpp
        deps/raknet/bit_stream.cpp
        deps/raknet/get_time.cpp
        deps/raknet/raknet_types.cpp
        entity/gamerefs_entity/entity_context.cpp
        entity/gamerefs_entity/entity_registry.cpp
        entity/gamerefs_entity/stack_result_storage_entity.cpp
        entity/gamerefs_entity/weak_storage_entity.cpp
        entity/utilities/synched_actor_data_access.cpp
        locale/i18n.cpp
        locale/localization.cpp
        nbt/compound_tag.cpp
        nbt/list_tag.cpp
        nbt/nbt_io.cpp
        nbt/tag.cpp
        network/connection_request.cpp
        network/network_identifier.cpp
        network/packet/types/world/actor/serialized_abilities_data.cpp
        network/packet/crafting_data_packet.cpp
        network/packet/inventory_slot_packet.cpp
        network/packet/text_packet.cpp
        network/network_system.cpp
        network/packet.cpp
        network/server_network_handler.cpp
        network/sub_client_connection_request.cpp
        platform/assigned_thread.cpp
        platform/uuid.cpp
        resources/base_game_version.cpp
        resources/pack.cpp
        resources/pack_error.cpp
        resources/pack_manifest.cpp
        resources/pack_source.cpp
        resources/resource_pack.cpp
        resources/resource_pack_manager.cpp
        resources/resource_pack_stack.cpp
        server/server_instance.cpp
        server/commands/command.cpp
        server/commands/command_origin.cpp
        server/commands/command_origin_loader.cpp
        server/commands/command_output.cpp
        server/commands/command_registry.cpp
        server/commands/command_utils.cpp
        server/commands/minecraft_commands.cpp
        server/commands/standard/teleport_command.cpp
        textobject/text_object.cpp
        util/data_io.cpp
        util/index_set.cpp
        util/string_byte_input.cpp
        util/string_byte_output.cpp
        world/game_session.cpp
        world/minecraft.cpp
        world/actor/actor.cpp
        world/actor/actor_damage_source.cpp
        world/actor/actor_definition_identifier.cpp
        world/actor/actor_factory.cpp
        world/actor/mob.cpp
        world/actor/synched_actor_data.cpp
        world/actor/item/item_actor.cpp
        world/actor/player/abilities.cpp
        world/actor/player/player.cpp
        world/actor/player/player_inventory.cpp
        world/actor/player/player_item_in_use.cpp
        world/actor/player/serialized_skin.cpp
        world/actor/provider/actor_offset.cpp
        world/attribute/attribute_instance.cpp
        world/attribute/attribute_map.cpp
        world/attribute/mutable_attribute_with_context.cpp
        world/attribute/attribute_modifier.cpp
        world/inventory/network/item_stack_net_id_variant.cpp
        world/item/item.cpp
        world/item/item_descriptor.cpp
        world/item/item_instance.cpp
        world/item/item_stack.cpp
        world/item/item_stack_base.cpp
        world/item/vanilla_item_tags.cpp
        world/item/crafting/recipes.cpp
        world/item/enchanting/enchant.cpp
        world/item/registry/creative_group_info.cpp
        world/item/registry/creative_item_entry.cpp
        world/item/registry/creative_item_registry.cpp
        world/item/registry/item_registry.cpp
        world/item/registry/item_registry_ref.cpp
        world/level/block_source.cpp
        world/level/explosion.cpp
        world/level/level.cpp
        world/level/weather.cpp
        world/level/biome/registry/biome_registry.cpp
        world/level/block/bed_block.cpp
        world/level/block/block.cpp
        world/level/block/block_descriptor.cpp
        world/level/block/block_type.cpp
        world/level/block/campfire_block.cpp
        world/level/block/fire_block.cpp
        world/level/block/liquid_block.cpp
        world/level/block/vanilla_block_type_ids.cpp
        world/level/block/actor/beehive_block_actor.cpp
        world/level/block/actor/furnace_block_actor.cpp
        world/level/block/registry/block_type_registry.cpp
        world/level/block/states/block_state.cpp
        world/level/block/states/vanilla_block_states.cpp
        world/level/chunk/level_chunk.cpp
        world/level/dimension/dimension.cpp
        world/level/saveddata/maps/map_decoration.cpp
        world/level/saveddata/maps/map_item_tracked_actor.cpp
        world/level/saveddata/map_item_saved_data.cpp
        world/scores/objective.cpp
        world/scores/scoreboard.cpp
        world/scores/scoreboard_id.cpp
        world/scores/scoreboard_identity_ref.cpp
        world/scores/server_scoreboard.cpp
)
add_library(bedrock::bedrock ALIAS bedrock)
add_dependencies(bedrock bedrock_symbols)
target_include_directories(bedrock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_compile_definitions(bedrock PUBLIC ENTT_SPARSE_PAGE=2048)
target_compile_definitions(bedrock PUBLIC ENTT_PACKED_PAGE=128)
target_compile_definitions(bedrock PUBLIC ENTT_NO_MIXIN)
target_link_libraries(bedrock PUBLIC bedrock::symbols endstone::endstone aklomp::base64 boost::boost fmt::fmt EnTT::EnTT glm::glm magic_enum::magic_enum Microsoft.GSL::GSL nonstd::expected-lite)
if (MSVC)
    target_link_options(bedrock PRIVATE /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF)
    target_compile_options(bedrock PRIVATE /O2 /DNDEBUG /Zi /Gy)
endif ()
if (UNIX)
    target_link_options(bedrock PRIVATE -g)
    target_compile_options(bedrock PRIVATE -O2 -DNDEBUG -g -fms-extensions)
endif ()

