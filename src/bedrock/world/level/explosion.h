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

#include <optional>
#include <unordered_set>

#include "bedrock/shared_types/legacy/level_sound_event.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"

using BlockPosSet = std::unordered_set<BlockPos>;

class Explosion {
public:
    Explosion(BlockSource &, Actor *, const Vec3 &, float);
    ENDSTONE_HOOK bool explode(IRandom &random);
    void setFire(bool);
    [[nodiscard]] bool getFire() const;

private:
    Vec3 pos_;
    float radius_;
    BlockPosSet affected_blocks_;
    bool fire_;
    bool breaking_;
    bool allow_underwater_;
    bool can_toggle_block_;
    float damage_scaling_;
    bool ignore_block_explosion_resistance_;
    LevelEvent particle_type_;
    LevelSoundEvent sound_explosion_type_;
    ActorUniqueID source_id_;
    BlockSource &region_;
    float max_resistance_;
    std::optional<bool> in_water_override_;
    std::optional<int> total_damage_override_;
    float knockback_scaling_;
};
