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

#include "endstone/detail/block/block_data.h"

#include "bedrock/nbt/nbt_io.h"

namespace endstone::detail {

EndstoneBlockData::EndstoneBlockData(::Block &block) : block_(block) {}

std::string EndstoneBlockData::getType() const
{
    return block_.getLegacyBlock().getFullNameId();
}

std::string EndstoneBlockData::getBlockStates() const
{
    if (const auto *states = getHandle().getSerializationId().get("states")) {
        return NbtIo::toJson(*states).dump();
    }
    return "";
}

::Block &EndstoneBlockData::getHandle() const
{
    return block_;
}

}  // namespace endstone::detail
