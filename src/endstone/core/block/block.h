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
#include "bedrock/world/level/block_source.h"
#include "endstone/block/block.h"
#include "endstone/core/level/dimension.h"
#include "endstone/util/result.h"

namespace endstone::core {
class EndstoneBlock : public Block {
public:
    EndstoneBlock(BlockSource &block_source, BlockPos block_pos);
    [[nodiscard]] const BlockType &getType() const override;
    void setType(BlockTypeId type) override;
    void setType(BlockTypeId type, bool apply_physics) override;
    [[nodiscard]] NotNull<BlockData> getData() const override;
    void setData(const BlockData &data) override;
    void setData(const BlockData &data, bool apply_physics) override;
    NotNull<Block> getRelative(int offset_x, int offset_y, int offset_z) override;
    NotNull<Block> getRelative(BlockFace face) override;
    NotNull<Block> getRelative(BlockFace face, int distance) override;
    [[nodiscard]] NotNull<Dimension> getDimension() const override;
    [[nodiscard]] const Biome &getBiome() const override;
    [[nodiscard]] int getX() const override;
    [[nodiscard]] int getY() const override;
    [[nodiscard]] int getZ() const override;
    [[nodiscard]] Location getLocation() const override;
    [[nodiscard]] NotNull<BlockState> captureState() const override;

    [[nodiscard]] BlockPos getPosition() const;
    [[nodiscard]] ::Block &getMinecraftBlock() const;

    static NotNull<EndstoneBlock> at(BlockSource &block_source, BlockPos block_pos);
    static BlockFace toBlockFace(Direction::Type direction);

protected:
    [[nodiscard]] BlockSource &getBlockSource() const;

    NotNull<EndstoneDimension> dimension_;
    BlockPos block_pos_;
};
}  // namespace endstone::core
