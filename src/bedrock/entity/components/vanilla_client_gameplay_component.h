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

struct VanillaClientGameplayComponent {
    bool operator==(const VanillaClientGameplayComponent &) const;
    bool operator!=(const VanillaClientGameplayComponent &) const;
    int sprint_trigger_time;
    int jump_riding_ticks;
    int jump_trigger_time;
    int fly_trigger_source;
    int scaffolding_drop_held_time;
    bool was_change_height;
    bool was_jumping;
    bool was_sneaking;
    bool was_running;
    bool emoting_last_tick;
    bool sprinting_on_input;
    bool boating_input_switch_requested;
    bool minecart_input_switch_requested;
    Vec3 last_delta;
    Vec3 last_pos;
    Vec3 previous_riding_position;
    float previous_riding_distance;
    std::uint64_t start_riding_tick;
};
BEDROCK_STATIC_ASSERT_SIZE(VanillaClientGameplayComponent, 80, 80);
