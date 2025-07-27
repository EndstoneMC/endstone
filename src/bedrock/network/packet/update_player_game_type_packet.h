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

#include "bedrock/network/packet.h"
#include "bedrock/network/packet/player_input_tick.h"

struct UpdatePlayerGameTypePacketInfo {
    static constexpr auto PACKET_NAME = "UpdatePlayerGameTypePacket";
    static constexpr auto PACKET_ID = MinecraftPacketIds::UpdatePlayerGameType;
    static constexpr auto DEFAULT_PACKET_SERIALIZATION_MODE = SerializationMode::SideBySide_LogOnMismatch;
    static constexpr auto COMPRESSIBILITY = Compressibility::Compressible;
};

struct UpdatePlayerGameTypePacketPayload {
    UpdatePlayerGameTypePacketPayload();
    UpdatePlayerGameTypePacketPayload(GameType, const ActorUniqueID &, uint64_t);

    GameType player_game_type;
    ActorUniqueID target_player;
    PlayerInputTick tick;
};

class UpdatePlayerGameTypePacket
    : public SerializedPayloadPacket<UpdatePlayerGameTypePacketInfo, UpdatePlayerGameTypePacketPayload> {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
};
