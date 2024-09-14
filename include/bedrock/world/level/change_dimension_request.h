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

#include "bedrock/core/math/vec3.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/util/rotation_command_utils.h"

class ChangeDimensionRequest {
    enum class State : int {
        PrepareRegion = 0,
        WaitingForChunks = 0x1,
        WaitingForRespawn = 0x2,
    };

public:
    State state;                             // +0
    DimensionType from_dimension;            // +4
    DimensionType to_dimension;              // +8
    Vec3 from_position;                      // +12
    Vec3 to_position;                        // +24
    bool use_portal;                         // +36
    bool respawn;                            // +37
    std::unique_ptr<CompoundTag> agent_tag;  // +40 std::unique_ptr
};
