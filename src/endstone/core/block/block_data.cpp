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

#include "endstone/core/block/block_data.h"

#include "bedrock/nbt/nbt_io.h"

namespace endstone::core {

EndstoneBlockData::EndstoneBlockData(::Block &block) : block_(block) {}

std::string EndstoneBlockData::getType() const
{
    return block_.getBlockType().getName().getString();
}

BlockStates EndstoneBlockData::getBlockStates() const
{
    BlockStates result;
    if (const auto *states = getHandle().getSerializationId().get("states")) {
        if (states->getId() != Tag::Type::Compound) {
            throw std::invalid_argument("Unexpected tag type for block states in serialization id");
        }
        for (const auto &[key, variant] : *static_cast<const CompoundTag *>(states)) {
            const auto *state = variant.get();
            switch (state->getId()) {
            case Tag::Type::Byte:
                result.emplace(key, static_cast<const ByteTag *>(state)->data > 0);
                break;
            case Tag::Type::Int:
                result.emplace(key, static_cast<const IntTag *>(state)->data);
                break;
            case Tag::Type::String:
                result.emplace(key, static_cast<const StringTag *>(state)->data);
                break;
            default:
                throw std::invalid_argument("Unexpected tag type in block states");
            }
        }
    }
    return result;
}

std::uint32_t EndstoneBlockData::getRuntimeId() const
{
    return getHandle().getRuntimeId();
}

::Block &EndstoneBlockData::getHandle() const
{
    return block_;
}

}  // namespace endstone::core
