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

#include "bedrock/core/math/math.h"
#include "bedrock/core/math/vec2.h"

namespace RotationUtility {
inline void setRot(const Vec2 &new_rot, Vec2 &current_rot, Vec2 &previous_rot)
{
    current_rot.y = mce::Math::wrapDegrees(new_rot.y);
    current_rot.x = mce::Math::wrapDegrees(new_rot.x);
    previous_rot.y = mce::Math::wrapDegrees(previous_rot.y);
    previous_rot.x = mce::Math::wrapDegrees(previous_rot.x);
}
};  // namespace RotationUtility
