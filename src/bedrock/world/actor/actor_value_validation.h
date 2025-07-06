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

#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/level/block_pos.h"

namespace ActorValueValidation {
constexpr float MIN_ALLOWED_FLOAT = -33554432.0;
constexpr float MAX_ALLOWED_FLOAT = 33554432.0;
constexpr int MIN_ALLOWED_INT = -2100000000;
constexpr int MAX_ALLOWED_INT = 2100000000;

bool validateBlockPos(const BlockPos &, const char *caller);
bool validateVec3Position(const Vec3 &, const char *caller);
bool validateVec3Velocity(const Vec3 &, const char *caller);
bool isLegalPlayerPosition(const Vec3 &, const char *caller);
bool validateVec2Position(const Vec2 &, const char *caller);
bool validateHeadRotationFloatValue(float, const char *caller);
}  // namespace ActorValueValidation
