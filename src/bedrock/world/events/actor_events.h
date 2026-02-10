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

#include "bedrock/entity/components/animation_event_component.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/actor/actor_definition_event.h"
#include "bedrock/world/actor/actor_heal_cause.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/effect/mob_effect_instance.h"
#include "bedrock/world/events/block_source_handle.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/item/equipment_slot.h"
#include "bedrock/world/item/item_helper.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/phys/hit_result.h"

struct ActorBeforeAcquireItemEvent {
    Actor &actor;
    Actor &item;
    int amount_acquired;
    ItemAcquisitionMethod acquire_method;
    WeakEntityRef secondary_actor_context;
};

struct ActorAcquiredItemEvent {
    Actor &actor;
    const ItemInstance item;
    int amount_acquired;
    ItemAcquisitionMethod acquire_method;
    WeakEntityRef secondary_actor_context;
};

struct ActorBeforeHurtEvent {
    const Actor &mEntity;
    const ActorDamageSource &mSource;
    float mDamage;
};

struct ActorBeforeHealEvent {
    const Actor &mEntity;
    ActorHealCause mCause;
    float mHealing;
};

struct ActorAddEffectEvent {
    WeakEntityRef entity;
    MobEffectInstance mob_effect;
};

struct ActorAnimationChangedEvent {
    WeakEntityRef entity;
    AnimationEventType type;
    AnimationEventState state;
};

struct ActorDefinitionTriggeredEvent {
    Actor &actor;
    const std::string event;
};

struct ActorDefinitionStartedEvent {
    Actor &actor;
    const std::string event;
    std::vector<ActorDefinitionModifier> &definition_modifiers;
};

struct ActorDefinitionEndedEvent {
    Actor &actor;
    const std::string event;
    const std::vector<ActorDefinitionModifier> definition_modifiers;
};

struct ActorGriefingBlockEvent {
    WeakRef<EntityContext> actor_context;
    gsl::not_null<const Block *> block;
    Vec3 pos;
    std::shared_ptr<BlockSourceHandle> block_source_handle;
};

struct ActorHurtEvent {
    WeakEntityRef entity;
    std::shared_ptr<ActorDamageSource> source;
    float damage;
    int absorbed_damage;
};

struct ActorHealEvent {
    WeakEntityRef mEntity;
    ActorHealCause mCause;
    float mHealing;
};

struct ActorHealthChangedEvent {
    WeakEntityRef entity;
    float old_value;
    float new_value;
};

struct ActorKilledEvent {
    WeakRef<EntityContext> actor_context;
    std::shared_ptr<ActorDamageSource> source;
};

struct ActorDiedEvent {
    WeakEntityRef entity;
    std::shared_ptr<ActorDamageSource> source;
    ActorType damage_child_type;
};

struct ActorRemovedEvent {
    WeakEntityRef entity;
};

struct ActorRemoveEffectEvent {
    WeakEntityRef entity;
    const MobEffectInstance mob_effect;
};

struct ActorUseItemEvent {
    Actor &actor;
    const ItemInstance item_used;
    ItemUseMethod item_use_method;
};

struct KnockBackEvent {
    WeakRef<EntityContext> actor;
};

struct MountTamingEvent {
    WeakRef<EntityContext> passenger;
    WeakRef<EntityContext> mount;
};

struct ProjectileHitEvent {
    Actor &projectile;
    const HitResult hit_result;
};

struct ActorAttackEvent {
    enum class AttackType : int {
        Melee = 0,
        Ranged = 1,
    };
    WeakEntityRef entity;
    WeakEntityRef target;
    int damage;
    AttackType attack_type;
};

struct ActorEquippedArmorEvent {
    WeakEntityRef entity;
    const ItemInstance armor;
    ArmorSlot slot;
};

struct ActorPlacedItemEvent {
    WeakEntityRef entity;
    const ItemInstance item;
};

struct ActorDroppedItemEvent {
    WeakEntityRef entity;
    const ItemInstance item;
    WeakEntityRef item_entity;
};

struct ActorCarriedItemChangedEvent {
    WeakEntityRef entity;
    const ItemInstance previously_carried_item;
    const ItemInstance carried_item;
    HandSlot hand_slot;
};

struct ActorStopRidingEvent {
    WeakEntityRef passenger;
    WeakEntityRef vehicle;
    bool exit_from_passenger;
    bool actor_is_being_destroyed;
    bool switching_vehicles;
};

struct ActorStartRidingEvent {
    WeakEntityRef passenger;
    WeakEntityRef vehicle;
};

struct ActorItemEventBeforeDroppedItem {
    WeakRef<EntityContext> actor;
    std::shared_ptr<ActorDamageSource> source;
    std::vector<ItemStack> items;
};

struct ActorItemEventAfterDroppedItem {
    WeakRef<EntityContext> actor;
    std::shared_ptr<ActorDamageSource> source;
    std::vector<ItemStack> items;
    std::vector<WeakRef<EntityContext>> item_actors;
};

template <typename Result>
struct ActorGameplayEvent;

template <>
struct ActorGameplayEvent<void>
    : ConstEventVariant<ActorAcquiredItemEvent, ActorAnimationChangedEvent, ActorAttackEvent,
                        ActorCarriedItemChangedEvent, ActorDefinitionTriggeredEvent, ActorDefinitionEndedEvent,
                        ActorDiedEvent, ActorDroppedItemEvent, ActorEquippedArmorEvent, ActorHurtEvent, ActorHealEvent,
                        ActorHealthChangedEvent, ActorKilledEvent, ActorPlacedItemEvent, ActorRemovedEvent,
                        ActorRemoveEffectEvent, ActorStartRidingEvent, ActorUseItemEvent, KnockBackEvent,
                        MountTamingEvent, ActorItemEventAfterDroppedItem> {};

template <>
struct ActorGameplayEvent<CoordinatorResult>
    : ConstEventVariant<ProjectileHitEvent, ActorGriefingBlockEvent, ActorStopRidingEvent> {};

template <typename Result>
struct MutableActorGameplayEvent;

template <>
struct MutableActorGameplayEvent<void> : MutableEventVariant<ActorItemEventBeforeDroppedItem> {};

template <>
struct MutableActorGameplayEvent<CoordinatorResult>
    : MutableEventVariant<ActorDefinitionStartedEvent, ActorAddEffectEvent, ActorBeforeAcquireItemEvent,
                          ActorBeforeHealEvent, ActorBeforeHurtEvent> {};
