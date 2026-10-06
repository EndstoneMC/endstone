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

#include <memory>
#include <unordered_map>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/forward.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/redstone/circuit/components/base_circuit_component.h"
#include "bedrock/world/redstone/circuit/components/circuit_component_list.h"

class BlockSource;

struct ChunkCircuitComponentList {
    bool should_evaluate;
    struct Item {
        BaseCircuitComponent *component;
        BlockPos pos;
        RedstoneLogicExecutionFlags cached_execution_flags;
    };
    std::vector<Item> components;
};

class CircuitSceneGraph {
public:
    using ComponentMap = std::unordered_map<BlockPos, std::unique_ptr<BaseCircuitComponent>>;
    using ComponentsPerPosMap = std::unordered_map<BlockPos, CircuitComponentList>;
    using ComponentsPerChunkMap = std::unordered_map<BlockPos, ChunkCircuitComponentList>;

    CircuitSceneGraph();
    ENDSTONE_HOOK void update(BlockSource *region);
    BaseCircuitComponent *getBaseComponent(const BlockPos &pos)
    {
        auto it = all_components_.find(pos);
        if (it == all_components_.end()) {
            return nullptr;
        }
        return it->second.get();
    }

private:
    ComponentMap all_components_;
    ComponentsPerChunkMap active_components_per_chunk_;
    ComponentsPerPosMap power_association_map_;
    class PendingEntry {
    public:
        BaseCircuitComponent *raw_component_ptr;
        std::unique_ptr<BaseCircuitComponent> component;
        BlockPos pos;
    };
    std::unordered_map<BlockPos, PendingEntry> pending_adds_;
    std::unordered_map<BlockPos, PendingEntry> pending_updates_;
    std::unordered_map<BlockPos, std::vector<BlockPos>> components_to_re_evaluate_;
    std::vector<PendingEntry> pending_removes_;
};
BEDROCK_STATIC_ASSERT_SIZE(CircuitSceneGraph, 408, 264);
