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

#include "bedrock/bedrock.h"
#include "bedrock/world/level/dimension/chunk_build_order_policy_base.h"

class ChunkBuildOrderPolicy : public ChunkBuildOrderPolicyBase {
public:
    ChunkBuildOrderPolicy();
    ~ChunkBuildOrderPolicy() override;
    [[nodiscard]] std::int32_t getChunkRebuildPriority(const ChunkPos &cp) const override;
    std::uint32_t registerForUpdates() override;
    void unregisterForUpdates(std::uint32_t handle) override;
    void setPlayerInfluence(std::uint32_t handle, const ChunkPos &player_position,
                            const Vec3 &player_movement_direction) override;
    void setTickingAreaInfluence(std::uint32_t handle, const ChunkPos &ticking_area_position, int size_x, int size_z,
                                 bool is_circle, bool preload) override;
    ENDSTONE_HOOK void updateInfluences() override;
};
