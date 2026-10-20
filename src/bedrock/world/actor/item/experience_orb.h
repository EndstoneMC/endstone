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

#include "bedrock/world/actor/actor.h"

class ExperienceOrb : public Actor {
public:
    enum class DropType : uint8_t {
        NoType = 0,
        FromBlock = 1,
        FromMob = 2,
        FromPlayer = 3,
    };

    ENDSTONE_HOOK void playerTouch(Player &player) override;
    static void spawnOrbs(BlockSource &block_source, const Vec3 &pos, int xp_value, DropType drop_type, Player *owner);
};
