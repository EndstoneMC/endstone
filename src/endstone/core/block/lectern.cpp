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

#include "endstone/core/block/lectern.h"

#include <algorithm>

#include "bedrock/world/level/block/lectern_block.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/inventory/lectern_inventory.h"

namespace endstone::core {

EndstoneLectern::EndstoneLectern(const EndstoneBlock &block, ::LecternBlockActor &lectern, bool use_snapshot)
    : EndstoneContainerBase<Lectern>(
          block, lectern, use_snapshot, std::make_unique<EndstoneLecternInventory>([this]() -> ::LecternBlockActor & {
              return getLiveBlockActor<::LecternBlockActor>();
          }),
          std::make_unique<EndstoneLecternInventory>([this]() -> ::LecternBlockActor & { return getLectern(); }))
{
}

bool EndstoneLectern::update(bool force, bool apply_physics)
{
    if (!EndstoneContainerBase::update(force, apply_physics)) {
        return false;
    }
    if (getType() == "minecraft:lectern") {
        static_cast<const ::LecternBlock &>(getBlockSource().getBlock(block_pos_).getBlockType())
            .emitRedstonePulse(getBlockSource(), block_pos_);
    }
    return true;
}

int EndstoneLectern::getPage() const
{
    return getLectern().getPage();
}

void EndstoneLectern::setPage(int page)
{
    const auto total_pages = getLectern().getTotalPages();
    getLectern().setPage(std::clamp(page, 0, std::max(total_pages - 1, 0)));
    getLectern().setChanged();
}

}  // namespace endstone::core
