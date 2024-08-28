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
#include "endstone/detail/server.h"
#include "endstone/event/block/block_break_event.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"

using endstone::detail::EndstoneBlock;
using endstone::detail::EndstoneServer;

bool GameMode::destroyBlock(BlockPos const &pos, FacingID face)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstonePlayer();
    const auto block = EndstoneBlock::at(player.getHandle().getDimension().getBlockSourceFromMainChunkSource(), pos);
    endstone::BlockBreakEvent e{*block, player};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::destroyBlock, __FUNCDNAME__, this, pos, face);
}

InteractionResult GameMode::useItemOn(ItemStack &item, BlockPos const &at, FacingID face, Vec3 const &hit,
                                      Block const *target_block)
{
    InteractionResult result = {0};

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstonePlayer();
    const auto block = EndstoneBlock::at(player.getHandle().getDimension().getBlockSourceFromMainChunkSource(), at);
    endstone::PlayerInteractEvent e{
        player,
        *block,
        static_cast<endstone::BlockFace>(face),
        {hit.x, hit.y, hit.z},
    };
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return result;
    }

#if _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO_NAME(&GameMode::useItemOn, __FUNCDNAME__, result, this, item, at, face, hit,
                                         target_block);
#else
    result =
        ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::useItemOn, __FUNCDNAME__, this, item, at, face, hit, target_block);
#endif
    return result;
}

bool GameMode::interact(Actor &actor, Vec3 const &location)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstonePlayer();
    endstone::PlayerInteractActorEvent e{player, actor.getEndstoneActor()};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&GameMode::interact, __FUNCDNAME__, this, actor, location);
}
