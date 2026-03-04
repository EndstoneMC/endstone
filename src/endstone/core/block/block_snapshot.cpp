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

#include "endstone/core/block/block_snapshot.h"

#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"

namespace endstone::core {

EndstoneBlockSnapshot::EndstoneBlockSnapshot(std::unique_ptr<EndstoneBlockState> state)
    : state_(std::move(state))
{
}

std::string EndstoneBlockSnapshot::getType() const
{
    return state_->getType();
}

void EndstoneBlockSnapshot::setType(std::string type)
{
    state_->setType(std::move(type));
}

void EndstoneBlockSnapshot::setType(std::string type, bool /*apply_physics*/)
{
    state_->setType(std::move(type));
}

std::unique_ptr<BlockData> EndstoneBlockSnapshot::getData() const
{
    return state_->getData();
}

void EndstoneBlockSnapshot::setData(const BlockData &data)
{
    state_->setData(data);
}

void EndstoneBlockSnapshot::setData(const BlockData &data, bool /*apply_physics*/)
{
    state_->setData(data);
}

std::unique_ptr<Block> EndstoneBlockSnapshot::getRelative(int offset_x, int offset_y, int offset_z)
{
    return getDimension().getBlockAt(getX() + offset_x, getY() + offset_y, getZ() + offset_z);
}

std::unique_ptr<Block> EndstoneBlockSnapshot::getRelative(BlockFace face)
{
    return getRelative(face, 1);
}

std::unique_ptr<Block> EndstoneBlockSnapshot::getRelative(BlockFace face, int distance)
{
    return getRelative(EndstoneBlockFace::getOffsetX(face) * distance, EndstoneBlockFace::getOffsetY(face) * distance,
                       EndstoneBlockFace::getOffsetZ(face) * distance);
}

Dimension &EndstoneBlockSnapshot::getDimension() const
{
    return state_->getDimension();
}

int EndstoneBlockSnapshot::getX() const
{
    return state_->getX();
}

int EndstoneBlockSnapshot::getY() const
{
    return state_->getY();
}

int EndstoneBlockSnapshot::getZ() const
{
    return state_->getZ();
}

Location EndstoneBlockSnapshot::getLocation() const
{
    return state_->getLocation();
}

std::unique_ptr<BlockState> EndstoneBlockSnapshot::captureState() const
{
    const auto &block = static_cast<const EndstoneBlockData &>(*state_->getData()).getHandle();
    return std::make_unique<EndstoneBlockState>(state_->getDimension(), BlockPos(getX(), getY(), getZ()), block);
}

std::unique_ptr<Block> EndstoneBlockSnapshot::clone() const
{
    const auto &block = static_cast<const EndstoneBlockData &>(*state_->getData()).getHandle();
    auto cloned_state =
        std::make_unique<EndstoneBlockState>(state_->getDimension(), BlockPos(getX(), getY(), getZ()), block);
    return std::make_unique<EndstoneBlockSnapshot>(std::move(cloned_state));
}

}  // namespace endstone::core
