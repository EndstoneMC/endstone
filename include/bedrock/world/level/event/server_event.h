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

#include "bedrock/details.h"
#include "bedrock/world/actor/registry/entity_context.h"

template <typename Return>
struct ServerInstanceGameplayEvent;

struct ServerInstanceLeaveGameDoneEvent {};
struct ServerInstanceRequestResourceReload {};

struct ServerInstanceEventPlaceHolder {
    char pad[16];
};

template <>
struct ServerInstanceGameplayEvent<void> {
    std::variant<Details::ValueOrRef<ServerInstanceLeaveGameDoneEvent const>,     // 0
                 Details::ValueOrRef<ServerInstanceRequestResourceReload const>,  // 1
                 Details::ValueOrRef<ServerInstanceEventPlaceHolder const>>
        event;
};

template <typename Return>
struct MutableServerNetworkGameplayEvent;

struct ChatEvent {
    std::string message;                                         // +0
    WeakRef<EntityContext> sender;                               // +32
    std::optional<std::vector<WeakRef<EntityContext>>> exclude;  // +56
    std::string player_name;                                     // +88
    bool unknown2{true};                                         // +120
    bool unknown3{true};                                         // +121
};

template <>
struct MutableServerNetworkGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<ChatEvent>>  // 0
        event;
};
