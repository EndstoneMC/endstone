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
#include "bedrock/event/coordinator_result.h"
#include "bedrock/event/event_ref.h"
#include "bedrock/forward.h"

// TODO: check the size
struct LevelAddedActorEvent {
    char pad[32];
};

struct LevelBroadcastEvent {
    LevelEvent level_event;
    int x;
    int z;
    int known;
};

struct LevelSoundBroadcastEvent {
    Puv::Legacy::LevelSoundEvent sound_event;
    int x;
    int z;
    int known;
};

class DayCycleEventComponent;

struct LevelDayCycleEvent {
    DayCycleEventComponent &component;
};

struct LevelStartLeaveGameEvent {};

struct LevelGameRuleChangeEvent {
    char pad[56];
};
struct ScriptingInitializeEvent {
    char pad[48];
};

template <typename Return>
struct LevelGameplayEvent;

template <>
struct LevelGameplayEvent<void> {
    std::variant<Details::ValueOrRef<LevelAddedActorEvent const>,      //
                 Details::ValueOrRef<LevelBroadcastEvent const>,       //
                 Details::ValueOrRef<LevelSoundBroadcastEvent const>,  //
                 Details::ValueOrRef<LevelDayCycleEvent const>,        //
                 Details::ValueOrRef<LevelStartLeaveGameEvent const>,  //
#ifdef _WIN32
                 Details::ValueOrRef<LevelGameRuleChangeEvent const>,  // what is going on Mojang?
#endif
                 Details::ValueOrRef<ScriptingInitializeEvent const>>  //
        event;
};
static_assert(sizeof(LevelGameplayEvent<void>) == 72);

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
