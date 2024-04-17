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
#include "bedrock/forward.h"
#include "bedrock/world/level/event/coordinator_result.h"
#include "bedrock/world/level/event/event_ref.h"

struct LevelEventPlaceHolder {
    char pad[56];
};

struct LevelAddedActorEvent {};
struct LevelBroadcastEvent {};
struct LevelSoundBroadcastEvent {};
struct LevelDayCycleEvent {};
struct LevelStartLeaveGameEvent {};
struct LevelGameRuleChangeEvent {};
struct ScriptingInitializeEvent {};

template <typename Return>
struct LevelGameplayEvent;

template <>
struct LevelGameplayEvent<void> {
    std::variant<Details::ValueOrRef<LevelAddedActorEvent const>,      // 0
                 Details::ValueOrRef<LevelBroadcastEvent const>,       // 1
                 Details::ValueOrRef<LevelSoundBroadcastEvent const>,  // 2
                 Details::ValueOrRef<LevelDayCycleEvent const>,        // 3
                 Details::ValueOrRef<LevelStartLeaveGameEvent const>,  // 4
#ifdef _WIN32
                 Details::ValueOrRef<LevelGameRuleChangeEvent const>,  // 5 (windows) what is going on Mojang?
#endif
                 Details::ValueOrRef<ScriptingInitializeEvent const>,  // 6 (windows) or 5 (linux)
                 Details::ValueOrRef<LevelEventPlaceHolder const>>     //
        event;
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
