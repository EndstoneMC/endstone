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

#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/server/commands/rotation_command_utils.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_type.h"
#include "bedrock/world/level/dimension/dimension_interface.h"

class ChangeDimensionRequest {
public:
    int unknown1;                  // +0
    DimensionType from_dimension;  // +4
    DimensionType to_dimension;    // +8
    Vec3 from_position;            // +12
    Vec3 to_position;              // +24
    bool unknown2;                 // +36
    bool unknown3;                 // +37
    void *unknown4;                // +40 std::unique_ptr
};

class TeleportData {
public:
    Vec3 destination;
    bool should_stop_riding;
    int cause;
    ActorType entity_type;
};

class TeleportTarget {
public:
    std::optional<Vec2> rotation;
    std::optional<TeleportData> data;
    std::unique_ptr<ChangeDimensionRequest> change_request;
};

class TeleportCommand {
public:
    static TeleportTarget computeTarget(Actor &, Vec3, Vec3 *, AutomaticID<Dimension, int>,
                                        std::optional<RotationCommandUtils::RotationData> const &, int);
    static void TeleportCommand::applyTarget(Actor &, TeleportTarget, bool);
};