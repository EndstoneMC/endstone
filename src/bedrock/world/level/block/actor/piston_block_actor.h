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

#include "bedrock/world/level/block/actor/block_actor.h"

enum class PistonState : char {
    Retracted = 0,
    Expanding = 1,
    Expanded = 2,
    Retracting = 3,
};

class PistonBlockActor : public BlockActor {
public:
    ENDSTONE_HOOK void tick(BlockSource &region) override;

private:
    void _tryFixupStickyPistonArm(BlockSource &region);

    bool sticky_;  // +184
    float progress_;
    float last_progress_;
    bool was_pushed_backward_by_a_non_sticky_piston_;
    bool was_pulled_forward_by_a_sticky_piston_;
    PistonState old_state_;
    PistonState state_;
    PistonState new_state_;
    bool verify_arm_;
    bool should_verify_arm_type_;  // +202
    std::vector<BlockPos> attached_blocks_;
    std::optional<std::vector<BlockPos>> future_client_attached_blocks_;
    std::vector<BlockPos> break_blocks_;
};
