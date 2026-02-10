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
#include "bedrock/entity/enums/client_input_lock_category.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/events/client_joined_event_data.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/gamerefs/weak_ref.h"
#include "bedrock/input/input_mode.h"
#include "bedrock/input/scripting_input_button.h"
#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/actor/actor_swing_source.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/container.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/game_type.h"

struct PlayerAddEvent {
    WeakRef<EntityContext> player;
};
struct PlayerAddExpEvent {
    WeakRef<EntityContext> player;
    int exp;
};
struct PlayerAddLevelEvent {
    WeakRef<EntityContext> player;
    int add_level;
    int new_level;
};
enum class PlayerArmorExchangeEventTriggeredLocation : int {
    ServerPlayerSetArmor = 0,
    MobHurtArmor = 1,
};
struct PlayerArmorExchangeEvent {
    WeakRef<EntityContext> player;
    ArmorSlot armor_slot;
    ItemStack old_item;
    ItemStack item;
    PlayerArmorExchangeEventTriggeredLocation triggered_location;
};
struct PlayerDestroyBlockEvent {
    WeakRef<EntityContext> player;
    BlockPos block_pos;
    FacingID face;
    gsl::not_null<const Block *> block;
};
struct PlayerSayCommandEvent {
    WeakRef<EntityContext> player;
    std::string message;
};
struct PlayerUseNameTagEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> entity_named;
    std::optional<Bedrock::Safety::RedactableString> previous_name;
    Bedrock::Safety::RedactableString new_name;
};
struct PlayerDropItemEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> spawned_item_actor;
};
struct PlayerEatFoodEvent {
    WeakRef<EntityContext> player;
    ItemStack food_item;
};
struct PlayerGetExperienceOrbEvent {
    WeakRef<EntityContext> player;
    int experience_value;
};
struct PlayerDamageEvent {
    WeakRef<EntityContext> player;
    std::unique_ptr<ActorDamageSource> damage_source;
};
struct PlayerOpenContainerEvent {
    WeakEntityRef player;
    ContainerType container_type;
    const BlockPos block_pos;
    ActorUniqueID entity_unique_id;
};
struct PlayerCloseContainerEvent {};
struct PlayerHasInvalidContainerEvent {};
struct PlayerShootArrowEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> arrow;
    ItemStack weapon_item;
    ItemStack arrow_item;
};
struct PlayerSwingStartEvent {
    WeakEntityRef player;
    ItemStack held_item;
    ActorSwingSource swing_source;
};
struct PlayerInitialSpawnEvent {
    WeakRef<EntityContext> player;
};
struct PlayerRespawnEvent {
    WeakRef<EntityContext> player;
};
struct PlayerSleepStateChangeEvent {
    bool is_sleeping;
};
struct PlayerStopLoadingEvent {
    WeakRef<EntityContext> player;
};
struct PlayerInputModeChangeEvent {
    WeakRef<EntityContext> player;
    InputMode new_input_mode_used;
    InputMode previous_input_mode_used;
};
struct PlayerInteractEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> target_entity;
    ItemStack item;
};
struct PlayerInteractWithEntityBeforeEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> target_entity;
    ItemStack item;
};
struct PlayerInteractWithEntityAfterEvent {
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> target_entity;
    ItemStack before_item;
    ItemStack after_item;
};
struct PlayerInteractWithBlockBeforeEvent {
    WeakRef<EntityContext> player;
    Vec3 block_location;
    FacingID block_face;
    Vec3 face_location;
    ItemStack item;
    bool is_first_event;
};
struct PlayerInteractWithBlockAfterEvent {
    WeakRef<EntityContext> player;
    Vec3 block_location;
    FacingID block_face;
    Vec3 face_location;
    ItemStack before_item;
    ItemStack after_item;
    bool is_first_event;
};
struct PlayerUpdateInteractionEvent {
    enum class ActionType : int {
        ApproachEntity = 0,
        LeaveEntity = 1,
    };
    WeakRef<EntityContext> player;
    WeakRef<EntityContext> entity;
    ActionType action;
};
struct PlayerSelectedItemChangedEvent {
    ItemStack item;
};
struct PlayerDimensionChangeBeforeEvent {
    WeakRef<EntityContext> player;
    DimensionType from_dimension;
    Vec3 from_location;
    DimensionType to_dimension;
    Vec3 to_location;
};
struct PlayerDimensionChangeAfterEvent {
    WeakRef<EntityContext> player;
    DimensionType from_dimension;
    Vec3 from_location;
    DimensionType to_dimension;
    Vec3 to_location;
};
struct PlayerFormResponseEvent {
    WeakRef<EntityContext> player;
    std::uint32_t form_id;
    Json::Value form_response;
};
enum class PlayerFormCloseReason : int {
    UserClosed = 0,
    UserBusy = 1
};
struct PlayerFormCloseEvent {
    WeakRef<EntityContext> player;
    std::uint32_t form_id;
    PlayerFormCloseReason form_close_reason;
};
struct PlayerDisconnectEvent {
    WeakEntityRef player;
};
struct PlayerSkinLoadedClientEvent {
    WeakEntityRef player;
};
struct PlayerGameModeChangeEvent {
    WeakEntityRef player;
    GameType from_game_mode;
    GameType to_game_mode;
};
struct PlayerInputPermissionCategoryChangeEvent {
    WeakEntityRef player;
    ClientInputLockCategory category;
    bool state;
};
struct PlayerEmoteEvent {
    WeakEntityRef player;
    std::string emote_piece_id;
};
struct PlayerScriptInputEvent {
    WeakEntityRef player;
    ScriptingInputButton button;
    ScriptingInputButtonState new_state;
};

struct PlayerInventoryItemChangeEvent {
    WeakEntityRef player;
    ItemStack previous_item;
    ItemStack new_item;
    int slot_number;
    bool is_quantity_changed_only;
};
static_assert(sizeof(PlayerInventoryItemChangeEvent) == 336);

struct PlayerHotbarSelectedSlotChangeEvent {
    WeakEntityRef player;
    int previous_slot_selected;
    int new_slot_selected;
    ItemStack item;
};
static_assert(sizeof(PlayerHotbarSelectedSlotChangeEvent) == 184);

template <typename Return>
struct PlayerGameplayEvent;

template <>
struct PlayerGameplayEvent<void>
    : ConstEventVariant<
          PlayerSkinLoadedClientEvent, PlayerAddEvent, PlayerAddExpEvent, PlayerAddLevelEvent, PlayerArmorExchangeEvent,
          PlayerDestroyBlockEvent, PlayerUseNameTagEvent, PlayerDropItemEvent, PlayerEatFoodEvent, PlayerDamageEvent,
          PlayerDisconnectEvent, PlayerFormCloseEvent, PlayerFormResponseEvent, PlayerInputModeChangeEvent,
          PlayerInitialSpawnEvent, PlayerOpenContainerEvent, PlayerCloseContainerEvent, PlayerHasInvalidContainerEvent,
          PlayerShootArrowEvent, PlayerSwingStartEvent, PlayerRespawnEvent, PlayerSleepStateChangeEvent,
          PlayerStopLoadingEvent, PlayerUpdateInteractionEvent, PlayerSelectedItemChangedEvent,
          PlayerDimensionChangeBeforeEvent, PlayerDimensionChangeAfterEvent, PlayerInteractWithEntityAfterEvent,
          PlayerInteractWithBlockAfterEvent, PlayerEmoteEvent, PlayerScriptInputEvent, PlayerInventoryItemChangeEvent,
          PlayerHotbarSelectedSlotChangeEvent, PlayerInputPermissionCategoryChangeEvent> {};
static_assert(sizeof(PlayerGameplayEvent<void>) == 384);

template <>
struct PlayerGameplayEvent<CoordinatorResult>
    : ConstEventVariant<PlayerSayCommandEvent, PlayerGetExperienceOrbEvent, PlayerInteractEvent,
                        PlayerInteractWithEntityBeforeEvent, PlayerInteractWithBlockBeforeEvent> {};
BEDROCK_STATIC_ASSERT_SIZE(PlayerGameplayEvent<CoordinatorResult>, 232, 232);

template <typename Return>
struct MutablePlayerGameplayEvent;
template <>
struct MutablePlayerGameplayEvent<CoordinatorResult> : MutableEventVariant<PlayerGameModeChangeEvent> {};
BEDROCK_STATIC_ASSERT_SIZE(MutablePlayerGameplayEvent<CoordinatorResult>, 48, 48);
