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

#include "bedrock/core/math/vec3.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/gamerefs/weak_ref.h"
#include "bedrock/scripting/lifetime_registry/lifetime_scope.h"
#include "bedrock/scripting/lifetime_registry/smart_object_handle.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/level/storage/game_rules.h"

class Level;

struct LevelAddedActorEvent {
    gsl::not_null<Bedrock::NonOwnerPointer<Level>> level;  // +0
    WeakRef<EntityContext> actor;                          // +16
};
struct LevelBroadcastEvent {
    const LevelEvent type;
    const Vec3 pos;
    const int data;
};
struct LevelSoundBroadcastEvent {
    const LevelSoundEvent type;
    const Vec3 pos;
    const int data;
};
struct LevelDayCycleEvent {
    int time_tick_in_day;
};
struct LevelGameRuleChangeEvent {
    GameRules::GameRulesIndex rule;
    GameRule::Type type;
    GameRule::Value value;
};
struct LevelWeatherChangedEvent {
    const bool is_raining;
    const bool is_lightning;
    bool will_be_raining;
    bool will_be_lightning;
    int rain_time;
    int lightning_time;
};
struct ScriptingWorldInitializeEvent {
    Scripting::StrongTypedObjectHandle<ScriptModuleMinecraft::ScriptPropertyRegistry> registry;
    Scripting::StrongTypedObjectHandle<ScriptModuleMinecraft::ScriptBlockComponentRegistry> block_component_registry;
    Scripting::StrongTypedObjectHandle<ScriptModuleMinecraft::ScriptItemComponentRegistry> item_component_registry;
    Scripting::WeakLifetimeScope scope;
    bool final_event;
};
struct LevelStartLeaveGameEvent {
    gsl::not_null<Bedrock::NonOwnerPointer<Level>> level;
};

template <typename Return>
struct LevelGameplayEvent;

template <>
struct LevelGameplayEvent<void>
    : ConstEventVariant<LevelAddedActorEvent, LevelBroadcastEvent, LevelSoundBroadcastEvent, LevelDayCycleEvent,
                        LevelStartLeaveGameEvent, LevelGameRuleChangeEvent, ScriptingWorldInitializeEvent> {};

template <typename Return>
struct MutableLevelGameplayEvent;

template <>
struct MutableLevelGameplayEvent<CoordinatorResult> : MutableEventVariant<LevelWeatherChangedEvent> {};
