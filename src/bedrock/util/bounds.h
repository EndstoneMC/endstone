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

#include "bedrock/bedrock.h"
#include "bedrock/shared_types/versionless/world/pos.h"

struct Bounds {
    static Bounds forChunks(int min_x, int min_z, int max_x, int max_z)
    {
        Bounds bounds;
        bounds.min = {.x = min_x, .y = 0, .z = min_z};
        bounds.max = {.x = max_x, .y = 0, .z = max_z};
        bounds.dim = {.x = max_x - min_x + 1, .y = 1, .z = max_z - min_z + 1};
        bounds.area = bounds.dim.x * bounds.dim.z;
        bounds.volume = bounds.area;
        bounds.side = 16;
        return bounds;
    }

    Pos min;
    Pos max;
    Pos dim;
    int area;
    int volume;
    int side;
};
BEDROCK_STATIC_ASSERT_SIZE(Bounds, 48, 48);
