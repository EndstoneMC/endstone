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

#include "bedrock/world/level/block/campfire_block.h"

#include "bedrock/shared_types/legacy/level_sound_event.h"
#include "bedrock/world/actor/actor_sound_identifier.h"
#include "bedrock/world/events/gameevents/game_event.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/material/material.h"

bool CampfireBlock::tryLightFire(BlockSource &region, const BlockPos &pos, Actor *source_actor)
{
    if (region.getBlock(pos).getMaterial().isType(MaterialType::Water)) {
        return false;
    }

    const auto &block = region.getBlock(pos);
    if (!block.getState<bool>(VanillaStateIds::Extinguished)) {
        return false;
    }

    auto after = block.setState<bool>(VanillaStateIds::Extinguished, false);
    region.getLevel().broadcastSoundEvent(region, LevelSoundEvent::Ignite, pos, -1, ActorSoundIdentifier(), false);
    region.setBlock(pos, *after, UPDATE_ALL, nullptr, nullptr);
    region.postGameEvent(source_actor, GameEventRegistry::blockChange, pos, nullptr);
    return true;
}
