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

#include "bedrock/world/item/item.h"

#include <spdlog/spdlog.h>

#include "bedrock/server/commands/command_utils.h"
#include "bedrock/world/level/block/block.h"

CoordinatorResult Item::_sendTryPlaceBlockEvent(Block const &placement_block, BlockSource const &block_source,
                                                Actor const &actor, BlockPos const &pos, FacingID face,
                                                Vec3 const &click_pos) const
{
    spdlog::info("Item::_sendTryPlaceBlockEvent");
    spdlog::info("- Block: {}", placement_block.getLegacyBlock().getFullNameId());
    spdlog::info("- Actor: {}", CommandUtils::getActorName(actor));
    spdlog::info("- BlockPos: {}, {}, {}", pos.x, pos.y, pos.z);
    spdlog::info("- Face: {}", face);
    spdlog::info("- ClickPos: {}, {}, {}", click_pos.x, click_pos.y, click_pos.z);
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Item::_sendTryPlaceBlockEvent, this, placement_block, block_source, actor, pos,
                                       face, click_pos);
}
