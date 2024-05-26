// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/forward.h"
#include "bedrock/mce.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/network/protocol/packet.h"
#include "bedrock/network/syncher/synced_player_movement_settings.h"
#include "bedrock/server/packs/content_identity.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/game_type.h"
#include "bedrock/world/level/storage/level_settings.h"
#include "bedrock/world/math/vec2.h"
#include "bedrock/world/math/vec3.h"

class NetworkPermissions {
public:
    bool unknown;
};

class StartGamePacket : public Packet {
public:
    BEDROCK_API virtual void write(BinaryStream &) const;

private:
    mutable LevelSettings settings_;                                     // +48
    ActorUniqueID entity_id_;                                            // +1296
    ActorRuntimeID runtime_id_;                                          // +1304
    GameType game_type_;                                                 // +1312
    Vec3 position_;                                                      // +1316
    Vec2 rotation_;                                                      // +1328
    std::string level_id_;                                               // +1336
    std::string level_name_;                                             // +1368
    ContentIdentity template_content_identity_;                          // +1400
    mce::UUID world_template_id_;                                        // +1424
    bool is_trial_;                                                      // +1440
    SyncedPlayerMovementSettings movement_settings_;                     // +1444
    std::uint64_t level_current_time_;                                   // +1456
    int enchantment_seed_;                                               // +1464
    std::string multiplayer_correlation_id_;                             // +1472
    std::vector<ItemData> item_data_;                                    // +1504
    bool enable_item_stack_net_manager_;                                 // +1528
    std::string server_version_;                                         // +1536
    CompoundTag player_property_data_;                                   // +1568
    std::uint64_t server_block_type_registry_checksum_;                  // +1592
    bool server_enabled_client_side_generation_;                         // +1600
    bool block_network_ids_are_hashes_;                                  // +1601
    NetworkPermissions network_permissions_;                             // +1602
    std::vector<std::pair<std::string, CompoundTag>> block_properties_;  // +1608
};
