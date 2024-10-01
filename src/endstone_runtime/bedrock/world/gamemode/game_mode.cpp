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

#include "bedrock/world/gamemode/game_mode.h"

#include <spdlog/spdlog.h>

#include "bedrock/world/actor/player/player.h"
#include "endstone/detail/block/block.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/inventory/item_stack.h"
#include "endstone/detail/server.h"
#include "endstone/event/block/block_break_event.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"

using endstone::detail::EndstoneActor;
using endstone::detail::EndstoneBlock;
using endstone::detail::EndstoneItemStack;
using endstone::detail::EndstonePlayer;
using endstone::detail::EndstoneServer;

bool GameMode::destroyBlock(BlockPos const &pos, FacingID face)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstoneActor<EndstonePlayer>();
    auto &block_source = player.getHandle().getDimension().getBlockSourceFromMainChunkSource();
    if (const auto block = EndstoneBlock::at(block_source, pos)) {
        endstone::BlockBreakEvent e{*block.value(), player};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    else {
        server.getLogger().error(block.error());
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::destroyBlock, __FUNCDNAME__, this, pos, face);
}

InteractionResult GameMode::useItemOn(ItemStack &item, BlockPos const &at, FacingID face, Vec3 const &hit,
                                      Block const *target_block)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstoneActor<EndstonePlayer>();
    auto &block_source = player.getHandle().getDimension().getBlockSourceFromMainChunkSource();
    if (auto block = EndstoneBlock::at(block_source, at)) {
        std::unique_ptr<EndstoneItemStack> item_stack =
            item.isNull() ? nullptr : std::make_unique<EndstoneItemStack>(item);
        endstone::PlayerInteractEvent e{
            player,
            std::move(item_stack),
            std::move(block.value()),
            static_cast<endstone::BlockFace>(face),
            {hit.x, hit.y, hit.z},
        };
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return InteractionResult::Failure();
        }
    }
    else {
        server.getLogger().error(block.error());
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::useItemOn, __FUNCDNAME__, this, item, at, face, hit,
                                            target_block);
}

bool GameMode::interact(Actor &actor, Vec3 const &location)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstoneActor<EndstonePlayer>();
    endstone::PlayerInteractActorEvent e{player, actor.getEndstoneActor<EndstoneActor>()};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::interact, __FUNCDNAME__, this, actor, location);
}
