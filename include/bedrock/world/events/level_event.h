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

#include "bedrock/core/details.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/entity/entity_context.h"
#include "bedrock/world/events/coordinator_result.h"
#include "bedrock/world/events/event_variant.h"

class Level;

struct LevelEventPlaceHolder {
    char pad[120];
};

struct LevelAddedActorEvent {
    Bedrock::NonOwnerPointer<Level> level;  // +0
    WeakRef<EntityContext> actor;           // +16
};
struct LevelBroadcastEvent {};
struct LevelSoundBroadcastEvent {};
struct LevelDayCycleEvent {};
struct LevelStartLeaveGameEvent {};
struct LevelGameRuleChangeEvent {};
struct ScriptingInitializeEvent {};

template <typename Return>
struct LevelGameplayEvent;

template <>
struct LevelGameplayEvent<void>
    : public ConstEventVariant<LevelAddedActorEvent, LevelBroadcastEvent, LevelSoundBroadcastEvent, LevelDayCycleEvent,
                               LevelStartLeaveGameEvent,
#ifdef _WIN32
                               LevelGameRuleChangeEvent,
#endif
                               ScriptingInitializeEvent, LevelEventPlaceHolder> {
};

struct LevelWeatherChangedEvent {
    bool from_rain;
    bool from_lightning;
    bool to_rain;
    bool to_lightning;
    int rain_time;
    int lightning_time;
};

template <typename Return>
struct MutableLevelGameplayEvent;

template <>
struct MutableLevelGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<LevelWeatherChangedEvent>> event;
};
