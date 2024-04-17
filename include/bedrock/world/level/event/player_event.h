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

#include <variant>

#include "bedrock/details.h"
#include "bedrock/world/level/event/coordinator_result.h"

template <std::size_t N>
struct PlayerEventPlaceHolder {  // To ensure our variant is initialised with correct storage size
    char pad[N];
};

struct PlayerSkinLoadedClientEvent {};
struct PlayerAddEvent {};
struct PlayerAddExpEvent {};
struct PlayerAddLevelEvent {};
struct PlayerArmorExchangeEvent {};
struct PlayerDestroyBlockEvent {};
struct PlayerUseNameTagEvent {};
struct PlayerDropItemEvent {};
struct PlayerEatFoodEvent {};
struct PlayerDamageEvent {};
struct PlayerDisconnectEvent {};
struct PlayerFormCloseEvent {};
struct PlayerFormResponseEvent {};
struct PlayerInitialSpawnEvent {};
struct PlayerOpenContainerEvent {};
struct PlayerShootArrowEvent {};
struct PlayerRespawnEvent {};
struct PlayerStopLoadingEvent {};
struct PlayerUpdateInteractionEvent {};
struct PlayerSelectedItemChangedEvent {};
struct PlayerDimensionChangeBeforeEvent {};
struct PlayerDimensionChangeAfterEvent {};
struct PlayerInteractWithEntityAfterEvent {};
struct PlayerInteractWithBlockAfterEvent {};

struct PlayerSayCommandEvent {};
struct PlayerGetExperienceOrbEvent {};
struct PlayerInteractEvent {};
struct PlayerInteractWithEntityBeforeEvent {};
struct PlayerInteractWithBlockBeforeEvent {};

template <typename Return>
struct PlayerGameplayEvent;

template <>
struct PlayerGameplayEvent<void> {
    std::variant<Details::ValueOrRef<PlayerSkinLoadedClientEvent const>,         // 0
                 Details::ValueOrRef<PlayerAddEvent const>,                      // 1
                 Details::ValueOrRef<PlayerAddExpEvent const>,                   // 2
                 Details::ValueOrRef<PlayerAddLevelEvent const>,                 // 3
                 Details::ValueOrRef<PlayerArmorExchangeEvent const>,            // 4
                 Details::ValueOrRef<PlayerDestroyBlockEvent const>,             // 5
                 Details::ValueOrRef<PlayerUseNameTagEvent const>,               // 6
                 Details::ValueOrRef<PlayerDropItemEvent const>,                 // 7
                 Details::ValueOrRef<PlayerEatFoodEvent const>,                  // 8
                 Details::ValueOrRef<PlayerDamageEvent const>,                   // 9
                 Details::ValueOrRef<PlayerDisconnectEvent const>,               // 10
                 Details::ValueOrRef<PlayerFormCloseEvent const>,                // 11
                 Details::ValueOrRef<PlayerFormResponseEvent const>,             // 12
                 Details::ValueOrRef<PlayerInitialSpawnEvent const>,             // 13
                 Details::ValueOrRef<PlayerOpenContainerEvent const>,            // 14
                 Details::ValueOrRef<PlayerShootArrowEvent const>,               // 15
                 Details::ValueOrRef<PlayerRespawnEvent const>,                  // 16
                 Details::ValueOrRef<PlayerStopLoadingEvent const>,              // 17
                 Details::ValueOrRef<PlayerUpdateInteractionEvent const>,        // 18
                 Details::ValueOrRef<PlayerSelectedItemChangedEvent const>,      // 19
                 Details::ValueOrRef<PlayerDimensionChangeBeforeEvent const>,    // 20
                 Details::ValueOrRef<PlayerDimensionChangeAfterEvent const>,     // 21
                 Details::ValueOrRef<PlayerInteractWithEntityAfterEvent const>,  // 22
                 Details::ValueOrRef<PlayerInteractWithBlockAfterEvent const>,   // 23
                 Details::ValueOrRef<PlayerEventPlaceHolder<368> const>>
        event;
};

template <>
struct PlayerGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<PlayerSayCommandEvent const>,                // 1
                 Details::ValueOrRef<PlayerGetExperienceOrbEvent const>,          // 2
                 Details::ValueOrRef<PlayerInteractEvent const>,                  // 3
                 Details::ValueOrRef<PlayerInteractWithEntityBeforeEvent const>,  // 4
                 Details::ValueOrRef<PlayerInteractWithBlockBeforeEvent const>,   // 5
                 Details::ValueOrRef<PlayerEventPlaceHolder<216> const>>
        event;
};

struct PlayerGameModeChangeEvent {
    char pad[32];
};

template <typename Return>
struct MutablePlayerGameplayEvent;

template <>
struct MutablePlayerGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<PlayerGameModeChangeEvent>> event;
};
