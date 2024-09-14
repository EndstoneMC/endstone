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

#include <cstdint>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/actor/actor_runtime_id.h"

class MoveActorAbsoluteData {
public:
    union Header {
        std::uint8_t raw;
        struct {
            bool is_on_ground : 1;
            bool teleported : 1;
            bool force_move_local_entity : 1;
        };
    };
    ActorRuntimeID runtime_id;  // +0
    Header header;              // +8
    Vec3 pos;                   // +12
    std::int8_t rot_x;          // +24
    std::int8_t rot_y;          // +25
    std::int8_t rot_y_head;     // +26
    std::int8_t rot_y_body;     // +27
};
BEDROCK_STATIC_ASSERT_SIZE(MoveActorAbsoluteData, 32, 32);
