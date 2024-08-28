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
#include "endstone/detail/block/block.h"
#include "endstone/detail/server.h"
#include "endstone/event/block/block_place_event.h"

using endstone::detail::EndstoneBlock;
using endstone::detail::EndstoneServer;

CoordinatorResult Item::_sendTryPlaceBlockEvent(Block const &placement_block, BlockSource const &block_source,
                                                Actor const &actor, BlockPos const &pos, FacingID face,
                                                Vec3 const &click_pos) const
{
    const auto &server = entt::locator<EndstoneServer>::value();

    if (actor.isPlayer()) {
        auto &player = static_cast<const Player &>(actor).getEndstonePlayer();
        const auto block_against = EndstoneBlock::at(const_cast<BlockSource &>(block_source), pos);
        const auto block_face = static_cast<endstone::BlockFace>(face);
        const auto block_replaced = block_against->getRelative(block_face);
        endstone::BlockPlaceEvent e{*block_replaced, *block_against, player};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return CoordinatorResult::Deny;
        }
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL(&Item::_sendTryPlaceBlockEvent, this, placement_block, block_source, actor, pos,
                                       face, click_pos);
}
