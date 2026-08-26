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

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "bedrock/core/math/color.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/cerealize/core/serialization_mode.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/player/serialized_skin.h"

enum class PlayerListPacketType : std::uint8_t {
    Add = 0,
    Remove = 1,
};

struct PlayerListPacketPayload {
    struct RemoveEntry {
        PlayerListPacketType action;
        mce::UUID uuid;
    };

    struct AddEntry {
        PlayerListPacketType action;
        mce::UUID uuid;
        ActorUniqueID id;
        std::string name;
        std::string xuid;
        std::string platform_online_id;
        BuildPlatform build_platform;
        SerializedSkinRef skin;
        bool is_teacher;
        bool is_host;
        bool is_sub_client;
        mce::Color color;
    };

    std::vector<std::variant<RemoveEntry, AddEntry>> entries;
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerListPacketPayload::RemoveEntry, 24, 24);
BEDROCK_STATIC_ASSERT_SIZE(PlayerListPacketPayload::AddEntry, 176, 152);
BEDROCK_STATIC_ASSERT_SIZE(PlayerListPacketPayload, 24, 24);

class PlayerListPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
    PlayerListPacketPayload payload;
    SerializationMode serialization_mode{SerializationMode::CerealOnly};
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerListPacket, 80, 80);
