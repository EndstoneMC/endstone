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

#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/world/events/event_variant.h"
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
struct ActorGameplayEvent<void> : ConstEventVariant<ActorAcquiredItemEvent,         // 0
                                                    ActorAnimationChangedEvent,     // 1
                                                    ActorAttackEvent,               // 2
                                                    ActorCarriedItemChangedEvent,   // 3
                                                    ActorDefinitionTriggeredEvent,  // 4
                                                    ActorDefinitionEndedEvent,      // 5
                                                    ActorDiedEvent,                 // 6
                                                    ActorDroppedItemEvent,          // 7
                                                    ActorEquippedArmorEvent,        // 8
                                                    ActorHurtEvent,                 // 9
                                                    ActorHealthChangedEvent,        // 10
                                                    ActorKilledEvent,               // 11
                                                    ActorPlacedItemEvent,           // 12
                                                    ActorRemovedEvent,              // 13
                                                    ActorRemoveEffectEvent,         // 14
                                                    ActorStartRidingEvent,          // 15
                                                    ActorUseItemEvent,              // 16
                                                    KnockBackEvent,                 // 17
                                                    MountTamingEvent,               // 18
                                                    ActorEventPlaceHolder<ENDSTONE_VARIANT_WIN32_LINUX(304, 320)>> {};

struct ProjectileHitEvent {};
struct ActorGriefingBlockEvent {};
struct ActorStopRidingEvent {};

template <>
struct ActorGameplayEvent<CoordinatorResult> : ConstEventVariant<ProjectileHitEvent,       // 0
                                                                 ActorGriefingBlockEvent,  // 1
                                                                 ActorStopRidingEvent,     // 2
                                                                 ActorEventPlaceHolder<120>> {};
