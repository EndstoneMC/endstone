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

#include "bedrock/world/level/block/registry/block_type_registry.h"

class BlockStateRegistry {
    BlockStateRegistry() = default;

public:
    void registerBlockState(const BlockState &state)
    {
        block_state_map_[state.getName()] = &state;
    }

    void unregisterBlockStates()
    {
        block_state_map_.clear();
    }

    static BlockStateRegistry &get()
    {
        static BlockStateRegistry instance;
        return instance;
    }

    const BlockState *getState(const HashedString &name)
    {
        const auto it = block_state_map_.find(name);
        if (it == block_state_map_.end()) {
            return nullptr;
        }
        return it->second;
    }

    [[nodiscard]] size_t getNumStates() const
    {
        return block_state_map_.size();
    }

private:
    std::unordered_map<HashedString, const BlockState *> block_state_map_;
};
