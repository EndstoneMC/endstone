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

#include "bedrock/world/redstone/circuit/circuit_scene_graph.h"

class CircuitSystem {
public:
    static constexpr int NOT_DEFINED_STRENGTH = -1;

    CircuitSystem();
    [[nodiscard]] int getStrength(const BlockPos &pos)
    {
        if (auto *component = scene_graph_.getBaseComponent(pos); component) {
            return component->getStrength();
        }
        return NOT_DEFINED_STRENGTH;
    }

private:
    bool lock_graph_;
    CircuitSceneGraph scene_graph_;
    // struct LevelChunkTracking {
    //     BlockPos chunk_pos;
    // };
    // std::vector<LevelChunkTracking> added_level_chunk_;
    // bool has_been_evaluated_;
};
