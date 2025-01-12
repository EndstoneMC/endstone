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

#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/events/event_variant.h"

template <typename Return>
struct MutableServerNetworkGameplayEvent;

struct ChatEvent {
    std::string message;
    std::optional<std::string> filtered_message;
    WeakRef<EntityContext> sender;
    std::optional<std::vector<WeakRef<EntityContext>>> targets;
    std::string author;
    bool message_valid;
    bool allow_filtering;
};

struct IncomingPacketEvent {
    WeakRef<EntityContext> sender;
    MinecraftPacketIds packet_id;
    std::size_t packet_size;
};

struct OutgoingPacketEvent {
    std::vector<WeakRef<EntityContext>> recipients;
    MinecraftPacketIds packet_id;
};

template <>
struct MutableServerNetworkGameplayEvent<CoordinatorResult>
    : MutableEventVariant<ChatEvent, IncomingPacketEvent, OutgoingPacketEvent> {};
