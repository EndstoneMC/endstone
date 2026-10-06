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

#include "endstone/core/block/block_type.h"

#include "bedrock/world/level/block/bedrock_block_names.h"
#include "endstone/core/block/block_data.h"

namespace endstone::core {
EndstoneBlockType::EndstoneBlockType(const ::BlockType &block_type) : block_type_(block_type) {}

BlockTypeId EndstoneBlockType::getId() const
{
    return block_type_.getName().getString();
}

std::string EndstoneBlockType::getTranslationKey() const
{
    return block_type_.buildDescriptionId(block_type_.getDefaultState());
}

bool EndstoneBlockType::hasItemType() const
{
    return block_type_.getBlockItemId() != 0;
}

std::unique_ptr<BlockData> EndstoneBlockType::createBlockData() const
{
    auto &default_state = const_cast<::Block &>(block_type_.getDefaultState());
    return std::make_unique<EndstoneBlockData>(default_state);
}
const ::BlockType &EndstoneBlockType::getHandle() const
{
    return block_type_;
}
}  // namespace endstone::core
