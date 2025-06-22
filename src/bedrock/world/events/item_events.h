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

#include <gsl/gsl>

#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/item_instance.h"

struct ShapedRecipeTriggeredEvent {
    gsl::not_null<const Recipe *> crafting_recipe;
    bool is_auto_craft;
};
static_assert(sizeof(ShapedRecipeTriggeredEvent) == 16);

struct ItemDefinitionEventTriggeredEvent {
    ItemInstance item_instance;
    WeakEntityRef actor;
    std::string event_name;
};

struct ItemUseOnEvent {
    ItemInstance item_before_use;
    WeakEntityRef actor;
    FacingID face;
    Vec3 face_location;
    BlockPos block_position;
    bool is_first_event;
};
static_assert(sizeof(ItemUseOnEvent) == 184);

struct ItemUsedOnEvent : ItemUseOnEvent {
    ItemInstance item_after_use;
};
static_assert(sizeof(ItemUsedOnEvent) == 312);

struct ItemStartUseOnEvent {
    ItemInstance item_instance;
    WeakEntityRef actor;
    FacingID face;
    BlockPos block_position;
    BlockPos build_block_position;
};
static_assert(sizeof(ItemStartUseOnEvent) == 184);

struct ItemStopUseOnEvent {
    ItemInstance item_instance;
    WeakEntityRef actor;
    BlockPos block_position;
};
static_assert(sizeof(ItemStopUseOnEvent) == 168);

struct ItemUseEvent {
    ItemInstance item_instance;
    WeakEntityRef actor;
};
static_assert(sizeof(ItemUseEvent) == 152);

struct ItemChargeEvent {
    ItemInstance item_instance;
    WeakEntityRef actor;
    int use_duration;
};
static_assert(sizeof(ItemChargeEvent) == 160);

struct ItemStartUseEvent : ItemChargeEvent {};
static_assert(sizeof(ItemStartUseEvent) == 160);

struct ItemCompleteUseEvent : ItemChargeEvent {};
static_assert(sizeof(ItemCompleteUseEvent) == 160);

struct ItemReleaseUseEvent : ItemChargeEvent {};
static_assert(sizeof(ItemReleaseUseEvent) == 160);

struct ItemStopUseEvent : ItemChargeEvent {};
static_assert(sizeof(ItemStopUseEvent) == 160);

struct CraftUpdateResultItemClientEvent {
    WeakRef<EntityContext> actor;
    const ItemInstance item;
};
static_assert(sizeof(CraftUpdateResultItemClientEvent) == 152);

template <typename Return>
struct ItemGameplayEvent;

template <>
struct ItemGameplayEvent<void> : ConstEventVariant<ShapedRecipeTriggeredEvent, CraftUpdateResultItemClientEvent> {};

template <typename Return>
struct MutableItemGameplayEvent;

template <>
struct MutableItemGameplayEvent<CoordinatorResult>
    : MutableEventVariant<ItemDefinitionEventTriggeredEvent, ItemUseOnEvent, ItemUseEvent, ItemUsedOnEvent,
                          ItemStartUseOnEvent, ItemStopUseOnEvent, ItemStartUseEvent, ItemCompleteUseEvent,
                          ItemReleaseUseEvent, ItemStopUseEvent> {};
