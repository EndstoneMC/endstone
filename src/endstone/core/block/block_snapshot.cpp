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

#include "bedrock/world/level/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/server.h"

namespace endstone::core {

const BlockType &EndstoneBlockSnapshot::getType() const
{
    return *BlockType::get(placed_->getName().getString());
}

void EndstoneBlockSnapshot::setType(BlockTypeId type)
{
    setData(*EndstoneServer::getInstance().createBlockData(type));
}

void EndstoneBlockSnapshot::setType(BlockTypeId type, bool /*apply_physics*/)
{
    setType(type);
}

std::unique_ptr<BlockData> EndstoneBlockSnapshot::getData() const
{
    return std::make_unique<EndstoneBlockData>(*placed_);
}

void EndstoneBlockSnapshot::setData(const BlockData &data)
{
    placed_ = &static_cast<const EndstoneBlockData &>(data).getHandle();
}

void EndstoneBlockSnapshot::setData(const BlockData &data, bool /*apply_physics*/)
{
    setData(data);
}

std::unique_ptr<BlockState> EndstoneBlockSnapshot::captureState() const
{
    return std::make_unique<EndstoneBlockState>(getDimension(), getPosition(), *placed_);
}

std::unique_ptr<Block> EndstoneBlockSnapshot::clone() const
{
    return std::make_unique<EndstoneBlockSnapshot>(block_source_, block_pos_, *placed_);
}

}  // namespace endstone::core
