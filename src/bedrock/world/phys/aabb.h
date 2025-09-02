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

class AABB {
public:
    AABB() = default;
    AABB(float x_min, float y_min, float z_min, float x_max, float y_max, float z_max)
        : min(x_min, y_min, z_min), max(x_max, y_max, z_max)
    {
    }
    Vec3 min;
    Vec3 max;
};
