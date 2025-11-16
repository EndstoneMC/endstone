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

#include "bedrock/world/level/block_pos.h"

struct PortalCooldownDurationComponent {
    static constexpr int PLAYER_NON_INVULNERABLE_WAIT_TICKS = 80;
    void resetCurrentPortalCooldown();
    BlockPos portal_block_pos;
    int max_portal_cooldown_ticks;
    int current_portal_cooldown_ticks;
    int inside_portal_time_ticks;
    int max_portal_wait_time;
};
