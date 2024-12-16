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

#include "bedrock/world/level/block/block_legacy.h"

bool BlockLegacy::requiresCorrectToolForDrops() const
{
    return requires_correct_tool_for_drops;
}

bool BlockLegacy::isSolid() const
{
    return solid_;
}

float BlockLegacy::getThickness() const
{
    return thickness_;
}

float BlockLegacy::getTranslucency() const
{
    return translucency_;
}

const std::vector<HashedString> &BlockLegacy::getTags() const
{
    return tags_;
}

const std::string &BlockLegacy::getDescriptionId() const
{
    return description_id_;
}

const std::string &BlockLegacy::getFullNameId() const
{
    return name_info_.full_name.getString();
}

const std::string &BlockLegacy::getRawNameId() const
{
    return name_info_.raw_name.getString();
}

const std::string &BlockLegacy::getNamespace() const
{
    return name_info_.namespace_name;
}

const Block *BlockLegacy::getDefaultState() const
{
    return default_state_;
}

void BlockLegacy::forEachBlockPermutation(std::function<bool(Block const &)> callback) const
{
    for (const auto &block_permutation : block_permutations_) {
        if (block_permutation) {
            callback(*block_permutation);
        }
    }
}
