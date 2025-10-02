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

#include "bedrock/entity/gamerefs_entity/entity_context.h"

class ServerInstance;

struct ServerInstanceLeaveGameDoneEvent {
    gsl::not_null<Bedrock::NonOwnerPointer<ServerInstance>> server_instance;
};
struct ServerInstanceRequestResourceReload {
    gsl::not_null<Bedrock::NonOwnerPointer<ServerInstance>> server_instance;
};

template <typename Return>
struct ServerInstanceGameplayEvent;

template <>
struct ServerInstanceGameplayEvent<void>
    : ConstEventVariant<ServerInstanceLeaveGameDoneEvent, ServerInstanceRequestResourceReload> {};

struct ServerInstanceNotificationEvent;