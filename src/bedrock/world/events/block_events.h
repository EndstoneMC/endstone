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

#include <memory>
#include <string>

#include "bedrock/core/math/vec3.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/forward.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension.h"

struct ActorInsideBlockEvent {};
struct ActorStandOnBlockEvent {};
struct BlockPatternEvent {
    const ActorDefinitionIdentifier entity_type;
    const Vec3 pos;
    const WeakRef<EntityContext> entity;
    const std::shared_ptr<BlockSourceHandle> block_source_handle;
};
struct BlockRandomTickEvent {};
struct BlockPatternPreEvent : BlockPatternEvent {};
struct BlockPatternPostEvent : BlockPatternEvent {};
struct ChestBlockTryPairEvent {};
struct CraftUISetResultNameEvent {
    enum class BlockType : int {
        Anvil = 0,
    };
    const BlockType type;
    const std::string name;
};
struct PistonActionEvent {};
struct LeverActionEvent {};
struct ButtonPushEvent {};
struct PressurePlatePushEvent {};
struct PressurePlatePopEvent {};
struct TargetBlockHitEvent {};
struct TripWireTripEvent {};
struct ExplosionStartedEvent {
    std::unordered_set<BlockPos> blocks;  // +0
    Dimension &dimension;                 // +40
    WeakRef<EntityContext> source;        // +48
};
struct BlockTryDestroyByPlayerEvent {
    WeakRef<EntityContext> player;  // +0
    const Block &block;             // +24
    const BlockPos pos;             // +32
    ItemStack item_used;            // +48
};
struct BlockTryPlaceByPlayerEvent {
    WeakRef<EntityContext> player;      // +0
    const BlockPos pos;                 // +24
    const FacingID face;                // +36
    const Vec3 face_location;           // +40
    const Block &permutation_to_place;  // +56
};

template <typename Result>
struct BlockGameplayEvent;

template <>
struct BlockGameplayEvent<void> : ConstEventVariant<BlockPatternPostEvent> {};

template <>
struct BlockGameplayEvent<CoordinatorResult> : ConstEventVariant<ActorInsideBlockEvent,      // 0
                                                                 ActorStandOnBlockEvent,     // 1
                                                                 BlockPatternPreEvent,       // 2
                                                                 BlockRandomTickEvent,       // 3
                                                                 ChestBlockTryPairEvent,     // 4
                                                                 PistonActionEvent,          // 5
                                                                 LeverActionEvent,           // 6
                                                                 ButtonPushEvent,            // 7
                                                                 PressurePlatePushEvent,     // 8
                                                                 PressurePlatePopEvent,      // 9
                                                                 TargetBlockHitEvent,        // 10
                                                                 TripWireTripEvent,          // 11
                                                                 BlockTryPlaceByPlayerEvent  // 12
                                                                 > {};

template <>
struct BlockGameplayEvent<std::optional<std::string>> : ConstEventVariant<CraftUISetResultNameEvent> {};

template <typename Result>
struct MutableBlockGameplayEvent;

template <>
struct MutableBlockGameplayEvent<CoordinatorResult>
    : MutableEventVariant<ExplosionStartedEvent, BlockTryDestroyByPlayerEvent> {};
