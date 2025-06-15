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

#include <unordered_map>

#include "bedrock/network/packet/move_actor_absolute_data.h"
#include "bedrock/util/new_type.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/tick.h"

class Actor;

struct ForceSendSpatialDataValue : NewType<unsigned int> {};

class SpatialActorNetworkData {
public:
    Actor *actor;
    bool has_initialized_last_sent;
    bool auto_send;
    MoveActorAbsoluteData last_sent_move_data;
    MoveActorAbsoluteData last_received_move_data;
    ForceSendSpatialDataValue last_force_send_spatial_data_value;
    std::unordered_map<ActorUniqueID, Tick> last_spatial_update_ticks;
};
// static_assert(sizeof(SpatialActorNetworkData) == 144);
