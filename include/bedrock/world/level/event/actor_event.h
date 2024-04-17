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

#include "bedrock/details.h"
#include "bedrock/world/level/event/coordinator_result.h"
#include "endstone/endstone.h"

template <std::size_t N>
struct ActorEventPlaceHolder {
    char pad[N];
};

template <typename Return>
struct ActorGameplayEvent;

struct ActorAcquiredItemEvent {};
struct ActorAnimationChangedEvent {};
struct ActorAttackEvent {};
struct ActorCarriedItemChangedEvent {};
struct ActorDefinitionTriggeredEvent {};
struct ActorDefinitionEndedEvent {};
struct ActorDiedEvent {};
struct ActorDroppedItemEvent {};
struct ActorEquippedArmorEvent {};
struct ActorHurtEvent {};
struct ActorHealthChangedEvent {};
struct ActorKilledEvent {};
struct ActorPlacedItemEvent {};
struct ActorRemovedEvent {};
struct ActorRemoveEffectEvent {};
struct ActorStartRidingEvent {};
struct ActorUseItemEvent {};
struct KnockBackEvent {};
struct MountTamingEvent {};

template <>
struct ActorGameplayEvent<void> {
    std::variant<Details::ValueOrRef<ActorAcquiredItemEvent const>,         // 0
                 Details::ValueOrRef<ActorAnimationChangedEvent const>,     // 1
                 Details::ValueOrRef<ActorAttackEvent const>,               // 2
                 Details::ValueOrRef<ActorCarriedItemChangedEvent const>,   // 3
                 Details::ValueOrRef<ActorDefinitionTriggeredEvent const>,  // 4
                 Details::ValueOrRef<ActorDefinitionEndedEvent const>,      // 5
                 Details::ValueOrRef<ActorDiedEvent const>,                 // 6
                 Details::ValueOrRef<ActorDroppedItemEvent const>,          // 7
                 Details::ValueOrRef<ActorEquippedArmorEvent const>,        // 8
                 Details::ValueOrRef<ActorHurtEvent const>,                 // 9
                 Details::ValueOrRef<ActorHealthChangedEvent const>,        // 10
                 Details::ValueOrRef<ActorKilledEvent const>,               // 11
                 Details::ValueOrRef<ActorPlacedItemEvent const>,           // 12
                 Details::ValueOrRef<ActorRemovedEvent const>,              // 13
                 Details::ValueOrRef<ActorRemoveEffectEvent const>,         // 14
                 Details::ValueOrRef<ActorStartRidingEvent const>,          // 15
                 Details::ValueOrRef<ActorUseItemEvent const>,              // 16
                 Details::ValueOrRef<KnockBackEvent const>,                 // 17
                 Details::ValueOrRef<MountTamingEvent const>,               // 18
                 Details::ValueOrRef<ActorEventPlaceHolder<_WIN32_LINUX_(304, 320)> const>>
        event;
};

struct ProjectileHitEvent {};
struct ActorGriefingBlockEvent {};
struct ActorStopRidingEvent {};

template <>
struct ActorGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<ProjectileHitEvent const>,       // 0
                 Details::ValueOrRef<ActorGriefingBlockEvent const>,  // 1
                 Details::ValueOrRef<ActorStopRidingEvent const>,     // 2
                 Details::ValueOrRef<ActorEventPlaceHolder<120> const>>
        event;
};
