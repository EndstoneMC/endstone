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
public:
    enum class State : int {
        PrepareRegion,
        WaitingForChunks,
        WaitingForRespawn,
    };
    State state;
    DimensionType from_dimension;
    DimensionType to_dimension;
    Vec3 from_position;
    Vec3 to_position;
    bool use_portal;
    bool respawn;
    std::unique_ptr<CompoundTag> agent_tag;
};
