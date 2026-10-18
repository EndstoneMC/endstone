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

#include <functional>
#include <optional>
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/platform/brstd/flat_map.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/dimension/dimension_type.h"

class Player;

class PlayerLocationSender {
public:
    PlayerLocationSender(PacketSender &packet_sender, int simulation_distance_chunks,
                         float simulation_distance_multiply_factor);
    struct PlayerLocationData {
        std::optional<Vec3> position;
        DimensionType dimension;
        bool is_spectator;
    };
    static const PlayerLocationData NULL_PLAYER_LOCATION_DATA;
    using OptionalPosition = std::optional<Vec3>;
    void updatePlayersData(const std::vector<WeakEntityRef> &gameplay_users);
    void removePlayerData(const ActorUniqueID &player_id);
    void checkPlayerPairsAndMaybeSendPackets(const std::vector<WeakEntityRef> &gameplay_users);
    void sendPacketsHidingAllPlayers(const std::vector<WeakEntityRef> &gameplay_users);

private:
    ENDSTONE_HOOK void _forEachClientPlayerPair(
        const std::vector<WeakEntityRef> &gameplay_users,
        std::function<void(const UserEntityIdentifierComponent &, const Player &, const Player &)> action);
    void _updatePlayerData(const Player &player);
    void _checkPlayerPairAndMaybeSendPacket(const UserEntityIdentifierComponent &user_identifier,
                                            const Player &viewing_player, const Player &observed_player);
#ifdef _WIN32
    [[nodiscard]] bool _shouldSendPositionPacket(const Vec3 &viewing_player_position,
                                                 const DimensionType &viewing_player_dimension,
                                                 bool viewing_player_is_spectator,
                                                 const OptionalPosition &observed_player_pos_prev,
                                                 const PlayerLocationData &observed_player_position_new) const;
#elif __linux__
    [[nodiscard]] static bool _shouldSendPositionPacket(
        const Vec3 &viewing_player_position, DimensionType viewing_player_dimension, bool viewing_player_is_spectator,
        const OptionalPosition &observed_player_pos_prev, const PlayerLocationData &observed_player_position_new,
        float simulation_distance);  // Endstone: LTO promoted the parameters
#endif
    void _setCurrentPlayerLocationData(const ActorUniqueID &player_id, const PlayerLocationData &data);
    struct ActorUniqueIDCompare {
        bool operator()(const ActorUniqueID &lhs, const ActorUniqueID &rhs) const;
    };
    struct ActorUniqueIDPairCompare {
        bool operator()(const std::pair<ActorUniqueID, ActorUniqueID> &lhs,
                        const std::pair<ActorUniqueID, ActorUniqueID> &rhs) const;
    };
    brstd::flat_map<ActorUniqueID, PlayerLocationData, ActorUniqueIDCompare, std::vector<ActorUniqueID>,
                    std::vector<PlayerLocationData>>
        current_player_location_data_;
    brstd::flat_map<std::pair<ActorUniqueID, ActorUniqueID>, std::optional<Vec3>, ActorUniqueIDPairCompare,
                    std::vector<std::pair<ActorUniqueID, ActorUniqueID>>, std::vector<std::optional<Vec3>>>
        sent_player_data_;
    Bedrock::NonOwnerPointer<PacketSender> packet_sender_;
    float simulation_distance_;
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerLocationSender::PlayerLocationData, 24, 24);
BEDROCK_STATIC_ASSERT_SIZE(PlayerLocationSender, 128, 128);
