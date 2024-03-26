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
#include "bedrock/event/event_ref.h"

// TODO: check the size
struct LevelAddedActorEvent {
    char pad[40];
};
struct LevelBroadcastEvent {
    char pad[64];
};
struct LevelSoundBroadcastEvent {
    char pad[64];
};
struct LevelDayCycleEvent {
    char pad[64];
};
struct LevelStartLeaveGameEvent {
    char pad[64];
};
struct LevelGameRuleChangeEvent {
    char pad[64];
};
struct ScriptingInitializeEvent {
    char pad[56];
};

template <typename Return>
struct LevelGameplayEvent;

template <>
struct LevelGameplayEvent<void> {
    std::variant<Details::ValueOrRef<LevelAddedActorEvent const>, Details::ValueOrRef<LevelBroadcastEvent const>,
                 Details::ValueOrRef<LevelSoundBroadcastEvent const>, Details::ValueOrRef<LevelDayCycleEvent const>,
                 Details::ValueOrRef<LevelStartLeaveGameEvent const>,
                 Details::ValueOrRef<LevelGameRuleChangeEvent const>,
                 Details::ValueOrRef<ScriptingInitializeEvent const>>
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
