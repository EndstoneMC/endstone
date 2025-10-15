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

#include <map>
#include <string>

#include "bedrock/platform/threading/mutex_details.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_type.h"

class BlockPalette {
public:
    enum class PaletteType : int {
        Sequential = 0,
        Hashed = 1,
    };

    virtual ~BlockPalette() = 0;
    [[nodiscard]] virtual PaletteType getPaletteType() const = 0;
    virtual void appendBlock(const Block &) = 0;
    [[nodiscard]] virtual const Block &getBlock(const BlockRuntimeId &) const = 0;

protected:
    virtual void assignBlockNetworkId(const Block &, BlockRuntimeId) const = 0;

public:
    [[nodiscard]] std::size_t getNumBlockNetworkIds() const
    {
        return block_from_network_id_.size();
    }

private:
    Bedrock::Threading::Mutex legacy_block_states_conversion_warning_mutex_;    // +8
    std::set<std::pair<int, int>> legacy_block_states_conversion_warning_set_;  // +88
    std::vector<Block *> block_from_network_id_;                                // +104
    Level *level_;                                                              // +128
};
