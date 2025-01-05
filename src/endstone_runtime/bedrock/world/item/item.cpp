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
#include "endstone/detail/block/block_face.h"
#include "endstone/detail/block/block_state.h"
#include "endstone/detail/server.h"
#include "endstone/event/block/block_place_event.h"

using endstone::detail::EndstoneBlock;
using endstone::detail::EndstoneBlockFace;
using endstone::detail::EndstoneBlockState;
using endstone::detail::EndstonePlayer;
using endstone::detail::EndstoneServer;

std::int16_t Item::getId() const
{
    return id_;
}

const std::string &Item::getFullItemName() const
{
    return full_name_.getString();
}

const WeakPtr<BlockLegacy> &Item::getLegacyBlock() const
{
    return legacy_block_;
}

const std::vector<ItemTag> &Item::getTags() const
{
    return tags_;
}

ItemDescriptor Item::buildDescriptor(std::int16_t aux_value, const CompoundTag *) const
{
    return ItemDescriptor(*this, aux_value);
}

float Item::getFurnaceBurnIntervalMultipler() const
{
    return furnace_burn_interval_modifier_;
}

CoordinatorResult Item::_sendTryPlaceBlockEvent(Block const &placement_block, BlockSource const &block_source,
                                                Actor const &actor, BlockPos const &pos, FacingID face,
                                                Vec3 const &click_pos) const
{
    if (actor.isPlayer()) {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto &player = actor.getEndstoneActor<EndstonePlayer>();
        auto &dimension = block_source.getDimension().getEndstoneDimension();

        const auto block_face = static_cast<endstone::BlockFace>(face);

        auto block_placed = std::make_unique<EndstoneBlockState>(dimension, pos, const_cast<Block &>(placement_block));
        if (const auto block_replaced = EndstoneBlock::at(const_cast<BlockSource &>(block_source), pos)) {

            const auto opposite = EndstoneBlockFace::getOpposite(block_face);
            if (const auto block_against = block_replaced.value()->getRelative(opposite)) {

                endstone::BlockPlaceEvent e{std::move(block_placed), *block_replaced.value(), *block_against.value(),
                                            player};
                server.getPluginManager().callEvent(e);
                if (e.isCancelled()) {
                    return CoordinatorResult::Cancel;
                }
            }
            else {
                server.getLogger().error(block_against.error());
            }
        }
        else {
            server.getLogger().error(block_replaced.error());
        }
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL(&Item::_sendTryPlaceBlockEvent, this, placement_block, block_source, actor, pos,
                                       face, click_pos);
}
