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
#include <unordered_map>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/world/level/dimension/chunk_build_order_policy_base.h"

namespace endstone::runtime {
struct ChunkBuildOrderSnapshot;
}

class ChunkBuildOrderPolicy : public ChunkBuildOrderPolicyBase {
    friend struct endstone::runtime::ChunkBuildOrderSnapshot;  // Endstone

public:
    ChunkBuildOrderPolicy();
    ~ChunkBuildOrderPolicy() override;
    [[nodiscard]] ENDSTONE_HOOK std::int32_t getChunkRebuildPriority(const ChunkPos &cp) const override;
    std::uint32_t registerForUpdates() override;
    void unregisterForUpdates(std::uint32_t handle) override;
    void setPlayerInfluence(std::uint32_t handle, const ChunkPos &player_position,
                            const Vec3 &player_movement_direction) override;
    void setTickingAreaInfluence(std::uint32_t handle, const ChunkPos &ticking_area_position, int size_x, int size_z,
                                 bool is_circle, bool preload) override;
    ENDSTONE_HOOK void updateInfluences() override;

private:
    enum class HintType : int {
        Player = 0,
        TickingAreaBox = 1,
        TickingAreaCircle = 2,
    };

    struct Influence {
        ChunkPos position;
        Vec3 parameter;
        HintType type;
        [[nodiscard]] std::int32_t getPriority(const ChunkPos &cp) const;
    };

    std::unordered_map<unsigned int, Influence> position_map_;
    SpinLock chunk_pos_async_spin_lock_;
    std::vector<Influence> chunk_pos_and_direction_async_;
    std::uint32_t last_handle_;
};
BEDROCK_STATIC_ASSERT_SIZE(ChunkBuildOrderPolicy, 128, 104);
