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

#include "bedrock/entity/weak_entity_ref.h"

enum class HitResultType : int {
    TILE = 0,
    ENTITY = 1,
    ENTITY_OUT_OF_RANGE = 2,
    NO_HIT = 3,
};

class HitResult {
public:
    HitResult() = default;

private:
    Vec3 start_pos_{Vec3::ZERO};
    Vec3 ray_dir_{Vec3::ZERO};
    HitResultType type_{HitResultType::NO_HIT};
    FacingID facing_{0};
    BlockPos block_{BlockPos::ZERO};
    Vec3 pos_{Vec3::ZERO};
    WeakEntityRef entity_{nullptr};
    bool is_hit_liquid_{false};
    FacingID liquid_facing_{0};
    BlockPos liquid_{BlockPos::ZERO};
    Vec3 liquid_pos_{Vec3::ZERO};
    bool indirect_hit_{false};
};
