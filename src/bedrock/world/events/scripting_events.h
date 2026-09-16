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

#include <optional>
#include <string>

#include "bedrock/forward.h"
#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/scripting/lifetime_registry/lifetime_scope.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"

enum class WatchdogTerminateReason : int {
    Hang = 0,
    StackOverflow = 1,
};

struct BeforeWatchdogTerminateEvent {
    Scripting::WeakLifetimeScope scope;                 // +0
    WatchdogTerminateReason watchdog_terminate_reason;  // +16
};

struct ScriptModuleStartupEvent {
    ScriptModuleMinecraft::IScriptItemCustomComponentRegistry &item_component_registry;
    ScriptModuleMinecraft::ScriptBlockCustomComponentsRegistry &block_component_registry;
    ScriptModuleMinecraft::ScriptCustomSpawnRulesRegistry &spawn_rules_registry;
    ScriptModuleMinecraft::CustomCommandRegistry &custom_command_registry;
    ScriptModuleMinecraft::ScriptCustomDimensionRegistry &custom_dimension_registry;
    // TODO(fixme): check the name - 1.26.51 appended this reference.
    void *unknown_40_;
};
struct ScriptModuleShutdownEvent {};

struct BlockObject {
    gsl::not_null<BlockSource *> region;
    BlockPos pos;
};

struct ScriptCommandMessageEvent {
    std::string message_id;
    std::string message_value;
    Level const &level;
    std::optional<ActorUniqueID> source_actor;
    std::optional<BlockObject> block_object;
    std::optional<ActorUniqueID> initiator;
};

template <typename Result>
struct MutableScriptingGameplayEvent;

template <>
struct MutableScriptingGameplayEvent<CoordinatorResult> : MutableEventVariant<BeforeWatchdogTerminateEvent> {};

template <typename Result>
struct ScriptingGameplayEvent;

template <>
struct ScriptingGameplayEvent<CoordinatorResult>
    : ConstEventVariant<ScriptCommandMessageEvent, ScriptModuleStartupEvent, ScriptModuleShutdownEvent> {
    using ConstEventVariant<ScriptCommandMessageEvent, ScriptModuleStartupEvent,
                            ScriptModuleShutdownEvent>::ConstEventVariant;
};
