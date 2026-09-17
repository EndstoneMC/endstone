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

#include "bedrock/core/math/vec3.h"
#include "bedrock/world/level/dimension/dimension_type.h"

struct ActorDimensionTransferRequest {
    enum class TargetPolicy : int {
        LegacyAuto = 0,
        ForceExplicit = 1,
    };
    DimensionType from_id;
    DimensionType to_id;
    std::optional<Vec3> explicit_target;
    TargetPolicy target_policy;
};
