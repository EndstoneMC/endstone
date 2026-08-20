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

#include "bedrock/world/level/block/actor_inside_block.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/campfire_block.h"
#include "bedrock/world/level/block/fire_block.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_inside_block_event.h"
#include "endstone/runtime/hook.h"

namespace {
bool isSupportedDefaultBlock(const ::Block &block)
{
    const auto &name = block.getName().getString();
    return name == "minecraft:bubble_column" || name == "minecraft:cactus" || name == "minecraft:carrots" ||
           name == "minecraft:beetroot" || name == "minecraft:end_gateway" ||
           name == "minecraft:honey_block" || name == "minecraft:hopper" || name == "minecraft:pitcher_crop" ||
           name == "minecraft:potatoes" || name == "minecraft:powder_snow" || name == "minecraft:torchflower_crop" ||
           name == "minecraft:waterlily" || name == "minecraft:wheat" || name == "minecraft:web";
}

bool callActorInsideBlockEvent(BlockSource &region, const BlockPos &position, Actor &actor)
{
    endstone::ActorInsideBlockEvent event{actor.getEndstoneActor<endstone::Actor>(),
                                          endstone::core::EndstoneBlock::at(region, position)};
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(event);
    return event.isCancelled();
}
}  // namespace

bool BlockType::shouldTriggerEntityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    const bool should_trigger =
        ENDSTONE_HOOK_CALL_ORIGINAL(&BlockType::shouldTriggerEntityInside, this, region, position, actor);
    if (!should_trigger || !isSupportedDefaultBlock(region.getBlock(position))) {
        return should_trigger;
    }
    return !callActorInsideBlockEvent(region, position, actor);
}

void BigDripleafBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&BigDripleafBlock::entityInside, this, region, position, actor);
}

void ButtonBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&ButtonBlock::entityInside, this, region, position, actor);
}

void CampfireBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CampfireBlock::entityInside, this, region, position, actor);
}

void DetectorRailBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&DetectorRailBlock::entityInside, this, region, position, actor);
}

void EndPortalBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&EndPortalBlock::entityInside, this, region, position, actor);
}

void EyeblossomBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&EyeblossomBlock::entityInside, this, region, position, actor);
}

void FireBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&FireBlock::entityInside, this, region, position, actor);
}

void FrogSpawnBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&FrogSpawnBlock::entityInside, this, region, position, actor);
}

void PortalBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PortalBlock::entityInside, this, region, position, actor);
}

void BasePressurePlateBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&BasePressurePlateBlock::entityInside, this, region, position, actor);
}

void SweetBerryBushBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&SweetBerryBushBlock::entityInside, this, region, position, actor);
}

void TripWireBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&TripWireBlock::entityInside, this, region, position, actor);
}

void WitherRoseBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&WitherRoseBlock::entityInside, this, region, position, actor);
}

void SoulFireBlock::entityInside(BlockSource &region, BlockPos const &position, Actor &actor) const
{
    if (callActorInsideBlockEvent(region, position, actor)) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&SoulFireBlock::entityInside, this, region, position, actor);
}
