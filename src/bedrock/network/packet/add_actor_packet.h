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

#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/synched_actor_data.h"

struct SyncedAttribute {
    std::string name;
    float min_value;
    float current_value;
    float max_value;
};
BEDROCK_STATIC_ASSERT_SIZE(SyncedAttribute, 48, 40);

struct AddActorPacketPayload {
    ActorUniqueID entity_id;
    ActorRuntimeID runtime_id;
    std::string actor_type;
    Vec3 pos;
    Vec3 velocity;
    Vec2 rot;
    float y_head_rotation;
    float y_body_rotation;
    std::vector<SyncedAttribute> attributes;
    SynchedActorData::CopyableDataList data;
    // ...
};

class AddActorPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;
    AddActorPacketPayload payload;
    // ...
};
