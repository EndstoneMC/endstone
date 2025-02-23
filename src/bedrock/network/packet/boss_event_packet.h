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
#include "bedrock/network/packet/types/world/actor/ai/util/boss_event_data.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/actor/actor_unique_id.h"

class BossEventPacket : public Packet {
public:
    const int FLAG_DARKEN{1};
    const int FLAG_FOG{2};
    ActorUniqueID boss_id;
    ActorUniqueID player_id;
    BossEventUpdateType event_type;
    Bedrock::Safety::RedactableString name;
    float health_percent;
    BossBarColor color;
    BossBarOverlay overlay;
    bool darken_screen;
    bool create_world_fog;
};
