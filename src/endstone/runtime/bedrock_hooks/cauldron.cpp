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

#include "bedrock/world/level/block/cauldron_block.h"

#include <algorithm>

#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/cauldron_level_change_event.h"
#include "endstone/runtime/hook.h"

void CauldronBlock::setLiquidLevel(BlockSource &region, const BlockPos &pos, int liquid_level,
                                   CauldronLiquidType liquid_type) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    const auto &block = region.getBlock(pos);
    if (block.getName() != VanillaBlockTypeIds::Cauldron) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    const auto level = std::clamp(liquid_level, MIN_FILL_LEVEL, MAX_FILL_LEVEL);
    const auto &filled_block = *block.setState<int>(fill_level, level);
    const auto &new_block = *filled_block.setState<int>(cauldron_liquid, static_cast<int>(liquid_type));
    if (new_block.getRuntimeId() == block.getRuntimeId()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    auto block_handle = endstone::core::EndstoneBlock::at(region, pos);
    auto new_state = block_handle->captureState();
    new_state->setData(endstone::core::EndstoneBlockData(const_cast<::Block &>(new_block)));
    endstone::CauldronLevelChangeEvent event{
        block_handle,
        nullptr,
        endstone::CauldronLevelChangeEvent::ChangeReason::Unknown,
        new_state,
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }

    const auto &final_block =
        static_cast<const endstone::core::EndstoneBlockData &>(*event.getNewState()->getData()).getHandle();
    if (final_block.getName() != VanillaBlockTypeIds::Cauldron) {
        event.getNewState()->update(true);
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos,
                                final_block.getState<int>(fill_level),
                                static_cast<CauldronLiquidType>(final_block.getState<int>(cauldron_liquid)));
}
