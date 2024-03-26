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

// TODO: check the size
struct PlayerSkinLoadedClientEvent {
    char pad[24];
};
struct PlayerAddEvent {
    char pad[24];
};
struct PlayerAddExpEvent {
    char pad[32];
};
struct PlayerAddLevelEvent {
    char pad[32];
};
struct PlayerArmorExchangeEvent {
    char pad[360];
};
struct PlayerDestroyBlockEvent {
    char pad[32];
};
struct PlayerUseNameTagEvent {
    char pad[184];
};
struct PlayerDropItemEvent {
    char pad[48];
};
struct PlayerEatFoodEvent {
    char pad[184];
};
struct PlayerDamageEvent {
    char pad[32];
};
struct PlayerDisconnectEvent {
    char pad[24];
};
struct PlayerFormCloseEvent {
    char pad[32];
};
struct PlayerFormResponseEvent {
    char pad[48];
};
struct PlayerInitialSpawnEvent {
    char pad[24];
};
struct PlayerOpenContainerEvent {
    char pad[48];
};
struct PlayerShootArrowEvent {
    char pad[368];
};
struct PlayerRespawnEvent {
    char pad[24];
};
struct PlayerStopLoadingEvent {
    char pad[24];
};
struct PlayerUpdateInteractionEvent {
    char pad[56];
};
struct PlayerSelectedItemChangedEvent {
    char pad[160];
};
struct PlayerDimensionChangeBeforeEvent {
    char pad[56];
};
struct PlayerDimensionChangeAfterEvent {
    char pad[56];
};
struct PlayerInteractWithEntityAfterEvent {
    char pad[208];
};
struct PlayerInteractWithBlockAfterEvent {
    char pad[208];
};

template <typename Return>
struct PlayerGameplayEvent {};

template <>
struct PlayerGameplayEvent<void> {
    std::variant<Details::ValueOrRef<PlayerSkinLoadedClientEvent const>, Details::ValueOrRef<PlayerAddEvent const>,
                 Details::ValueOrRef<PlayerAddExpEvent const>, Details::ValueOrRef<PlayerAddLevelEvent const>,
                 Details::ValueOrRef<PlayerArmorExchangeEvent const>,
                 Details::ValueOrRef<PlayerDestroyBlockEvent const>, Details::ValueOrRef<PlayerUseNameTagEvent const>,
                 Details::ValueOrRef<PlayerDropItemEvent const>, Details::ValueOrRef<PlayerEatFoodEvent const>,
                 Details::ValueOrRef<PlayerDamageEvent const>, Details::ValueOrRef<PlayerDisconnectEvent const>,
                 Details::ValueOrRef<PlayerFormCloseEvent const>, Details::ValueOrRef<PlayerFormResponseEvent const>,
                 Details::ValueOrRef<PlayerInitialSpawnEvent const>,
                 Details::ValueOrRef<PlayerOpenContainerEvent const>, Details::ValueOrRef<PlayerShootArrowEvent const>,
                 Details::ValueOrRef<PlayerRespawnEvent const>, Details::ValueOrRef<PlayerStopLoadingEvent const>,
                 Details::ValueOrRef<PlayerUpdateInteractionEvent const>,
                 Details::ValueOrRef<PlayerSelectedItemChangedEvent const>,
                 Details::ValueOrRef<PlayerDimensionChangeBeforeEvent const>,
                 Details::ValueOrRef<PlayerDimensionChangeAfterEvent const>,
                 Details::ValueOrRef<PlayerInteractWithEntityAfterEvent const>,
                 Details::ValueOrRef<PlayerInteractWithBlockAfterEvent const>>
        event;
};
