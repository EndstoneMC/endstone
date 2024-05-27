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

#include <cstdint>
#include <unordered_set>

#include "bedrock/network/packet/move_actor_absolute_data.h"
#include "bedrock/world/actor/actor_unique_id.h"

class Actor;

class SpatialActorNetworkData {
public:
    Actor *actor;                                         // +0
    bool initialised;                                     // +8
    bool auto_send;                                       // +9
    MoveActorAbsoluteData last_sent;                      // +16
    MoveActorAbsoluteData current;                        // +48
    std::unordered_set<ActorUniqueID> last_spatial_tick;  // +80
};
// static_assert(sizeof(SpatialActorNetworkData) == 144);
