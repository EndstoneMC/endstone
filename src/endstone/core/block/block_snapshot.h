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

#include "endstone/core/block/block.h"

class Block;

namespace endstone::core {

/**
 * @brief A live Block whose type and data are overridden by a captured permutation.
 *
 * Represents a block that doesn't yet exist in the world (e.g. a block about to be placed, before
 * placement actually occurs). Type/data reads return the captured permutation; everything positional
 * — getRelative(), getBiome(), coordinates, dimension — is inherited from EndstoneBlock and reads the
 * live world. setType()/setData() mutate the captured permutation only; they never write to the world.
 */
class EndstoneBlockSnapshot : public EndstoneBlock {
public:
    EndstoneBlockSnapshot(BlockSource &block_source, BlockPos block_pos, const ::Block &placed)
        : EndstoneBlock(block_source, block_pos), placed_(const_cast<::Block *>(&placed))
    {
    }

    [[nodiscard]] const BlockType &getType() const override;
    void setType(BlockTypeId type) override;
    void setType(BlockTypeId type, bool apply_physics) override;
    [[nodiscard]] std::unique_ptr<BlockData> getData() const override;
    void setData(const BlockData &data) override;
    void setData(const BlockData &data, bool apply_physics) override;
    [[nodiscard]] std::unique_ptr<BlockState> captureState() const override;
    [[nodiscard]] std::unique_ptr<Block> clone() const override;

private:
    ::Block *placed_;
};

}  // namespace endstone::core
