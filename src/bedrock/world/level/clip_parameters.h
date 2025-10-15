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

#include <functional>

#include "bedrock/core/math/vec3.h"

class Actor;
class Block;
class BlockSource;

enum class ShapeType : uint8_t {
    Outline = 0,
    Collision = 1,
    CollisionForCamera = 2,
};

namespace ClipUtils {
using ShouldCheckBlockFunction = std::function<bool(const BlockSource &, const Block &, bool)>;
};

struct ClipParameters {
    ClipParameters(const Vec3 &a, const Vec3 &b)
        : ClipParameters(a, b, [](const auto &, const auto &, auto) { return true; })
    {
    }
    ClipParameters(const Vec3 &a, const Vec3 &b, const ClipUtils::ShouldCheckBlockFunction &should_check_block)
        : a(a), b(b), should_check_block(should_check_block)
    {
    }
    const Vec3 &a;
    const Vec3 &b;
    Actor *actor = nullptr;
    bool liquid = false;
    bool full_only = false;
    bool stop_on_first_liquid_hit;
    bool ignore_border_block = false;
    ShapeType shape_type = ShapeType::Outline;
    int max_distance = 0;
    const ClipUtils::ShouldCheckBlockFunction should_check_block;
};
