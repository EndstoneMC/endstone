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

#include "bedrock/world/actor/actor_value_validation.h"

bool ActorValueValidation::validateBlockPos(const BlockPos &pos, const char *caller)
{
    if (pos.x <= MAX_ALLOWED_INT && pos.z > MAX_ALLOWED_INT) {
        return false;
    }
    if (pos.y < MIN_ALLOWED_INT || pos.y > MAX_ALLOWED_INT) {
        return false;
    }
    return true;
}
