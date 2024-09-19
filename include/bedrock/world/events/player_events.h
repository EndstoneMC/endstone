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

#include "bedrock/deps/json/value.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/gamerefs/weak_ref.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/game_type.h"

template <typename Return>
struct PlayerGameplayEvent;

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

enum class PlayerFormCloseReason : int {
    UserClosed = 0,
    UserBusy = 1
};

struct PlayerFormCloseEvent {
    WeakRef<EntityContext> player;
    int form_id;
    PlayerFormCloseReason form_close_reason;
};

struct PlayerFormResponseEvent {
    WeakRef<EntityContext> player;
    int form_id;
    Json::Value form_response;
};

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
// NOTE: keep it up to date to ensure the variant has the right size
struct PlayerInteractWithBlockAfterEvent {
    WeakRef<EntityContext> player;
    Vec3 block_location;
    FacingID block_face;
    Vec3 face_location;
    std::size_t before_item[sizeof(ItemStack) / 8];  // Endstone: ItemStack is not constructible currently
    std::size_t after_item[sizeof(ItemStack) / 8];
    bool is_first_event;
};
struct PlayerEmoteEvent {};
struct PlayerInputPermissionCategoryChangeEvent {};

template <>
struct PlayerGameplayEvent<void>
    : ConstEventVariant<PlayerSkinLoadedClientEvent, PlayerAddEvent, PlayerAddExpEvent, PlayerAddLevelEvent,
                        PlayerArmorExchangeEvent, PlayerDestroyBlockEvent, PlayerUseNameTagEvent, PlayerDropItemEvent,
                        PlayerEatFoodEvent, PlayerDamageEvent, PlayerDisconnectEvent, PlayerFormCloseEvent,
                        PlayerFormResponseEvent, PlayerInitialSpawnEvent, PlayerOpenContainerEvent,
                        PlayerShootArrowEvent, PlayerRespawnEvent, PlayerStopLoadingEvent, PlayerUpdateInteractionEvent,
                        PlayerSelectedItemChangedEvent, PlayerDimensionChangeBeforeEvent,
                        PlayerDimensionChangeAfterEvent, PlayerInteractWithEntityAfterEvent,
                        PlayerInteractWithBlockAfterEvent, PlayerEmoteEvent, PlayerInputPermissionCategoryChangeEvent> {
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerGameplayEvent<void>, 384, 384);

struct PlayerSayCommandEvent {};
struct PlayerGetExperienceOrbEvent {};
struct PlayerInteractEvent {};
struct PlayerInteractWithEntityBeforeEvent {};
// NOTE: keep it up to date to ensure the variant has the right size
struct PlayerInteractWithBlockBeforeEvent {
    WeakRef<EntityContext> player;
    Vec3 block_location;
    FacingID block_face;
    Vec3 face_location;
    std::size_t item[sizeof(ItemStack) / 8];  // Endstone: ItemStack is not constructible currently
    bool is_first_event;
};

template <>
struct PlayerGameplayEvent<CoordinatorResult>
    : ConstEventVariant<PlayerSayCommandEvent, PlayerGetExperienceOrbEvent, PlayerInteractEvent,
                        PlayerInteractWithEntityBeforeEvent, PlayerInteractWithBlockBeforeEvent> {};
BEDROCK_STATIC_ASSERT_SIZE(PlayerGameplayEvent<CoordinatorResult>, 232, 232);

struct PlayerGameModeChangeEvent {
    WeakRef<EntityContext> player;
    GameType from_game_mode;
    GameType to_game_mode;
};

template <typename Return>
struct MutablePlayerGameplayEvent;
template <>
struct MutablePlayerGameplayEvent<CoordinatorResult> : MutableEventVariant<PlayerGameModeChangeEvent> {};
BEDROCK_STATIC_ASSERT_SIZE(MutablePlayerGameplayEvent<CoordinatorResult>, 48, 48);
