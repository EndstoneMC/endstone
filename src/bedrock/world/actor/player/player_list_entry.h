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

#include "bedrock/core/math/color.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/platform/result.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/player/serialized_skin.h"

class Player;

class PlayerListEntry {
public:
    PlayerListEntry();
    PlayerListEntry(PlayerListEntry &&);
    PlayerListEntry &operator=(PlayerListEntry &&);
    explicit PlayerListEntry(mce::UUID);
    explicit PlayerListEntry(const Player &player);
    ~PlayerListEntry();
    Bedrock::Result<void> readRemove(ReadOnlyBinaryStream &);
    void writeRemove(BinaryStream &) const;
    Bedrock::Result<void> read(ReadOnlyBinaryStream &);
    void write(BinaryStream &) const;
    [[nodiscard]] PlayerListEntry clone() const;
    [[nodiscard]] PlayerListEntry cloneExceptSkin() const;
    ActorUniqueID id;
    mce::UUID uuid;
    std::string name;
    std::string xuid;
    std::string platform_online_id;
    BuildPlatform build_platform;
    SerializedSkinRef skin;
    mce::Color color;
    bool is_teacher;
    bool is_host;
    bool is_sub_client;

private:
    PlayerListEntry(const PlayerListEntry &);
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerListEntry, 168, 144);
