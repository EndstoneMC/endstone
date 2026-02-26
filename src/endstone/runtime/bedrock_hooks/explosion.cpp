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

#include "bedrock/world/level/explosion.h"

#include <optional>

#include "endstone/runtime/hook.h"

namespace endstone::runtime {
thread_local std::optional<Vec3> last_explosion_pos;

const std::optional<Vec3> &getLastExplosionPos()
{
    return last_explosion_pos;
}
}  // namespace endstone::runtime

bool Explosion::explode(IRandom &random)
{
    endstone::runtime::last_explosion_pos = pos_;
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&Explosion::explode, this, random);
    endstone::runtime::last_explosion_pos.reset();
    return result;
}
