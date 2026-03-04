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

#include "endstone/block/block.h"
#include "endstone/core/block/block_state.h"

namespace endstone::core {

/**
 * @brief A Block implementation backed by a captured BlockState.
 *
 * This allows representing a block that doesn't yet exist in the world (e.g., a placed block
 * before placement actually occurs). Read methods delegate to the captured state, while
 * getRelative() returns live blocks from the world.
 */
class EndstoneBlockSnapshot : public Block {
public:
    explicit EndstoneBlockSnapshot(std::unique_ptr<EndstoneBlockState> state);

    [[nodiscard]] std::string getType() const override;
    void setType(std::string type) override;
    void setType(std::string type, bool apply_physics) override;
    [[nodiscard]] std::unique_ptr<BlockData> getData() const override;
    void setData(const BlockData &data) override;
    void setData(const BlockData &data, bool apply_physics) override;
    std::unique_ptr<Block> getRelative(int offset_x, int offset_y, int offset_z) override;
    std::unique_ptr<Block> getRelative(BlockFace face) override;
    std::unique_ptr<Block> getRelative(BlockFace face, int distance) override;
    [[nodiscard]] Dimension &getDimension() const override;
    [[nodiscard]] int getX() const override;
    [[nodiscard]] int getY() const override;
    [[nodiscard]] int getZ() const override;
    [[nodiscard]] Location getLocation() const override;
    [[nodiscard]] std::unique_ptr<BlockState> captureState() const override;
    [[nodiscard]] std::unique_ptr<Block> clone() const override;

private:
    std::unique_ptr<EndstoneBlockState> state_;
};

}  // namespace endstone::core
