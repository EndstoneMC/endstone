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

#include <entt/core/type_info.hpp>

#include "bedrock/bedrock.h"
#include "bedrock/world/actor/hopper.h"
#include "bedrock/world/level/block_pos.h"

class HopperComponent : public Hopper {
private:
    BlockPos last_position_;
};
BEDROCK_STATIC_ASSERT_SIZE(HopperComponent, 160, 128);
#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<HopperComponent>::value() == 0x2C01093A);
#endif
