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

#include "bedrock/world/actor/player/player.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_break_event.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstoneActor;
using endstone::core::EndstoneBlock;
using endstone::core::EndstoneItemStack;
using endstone::core::EndstonePlayer;
using endstone::core::EndstoneServer;

InteractionResult GameMode::useItemOn(ItemStack &item, BlockPos const &at, FacingID face, Vec3 const &hit,
                                      Block const *target_block, bool is_first_event)
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

    return ENDSTONE_HOOK_CALL_ORIGINAL(&GameMode::useItemOn, this, item, at, face, hit, target_block, is_first_event);
}

bool GameMode::interact(Actor &actor, Vec3 const &location)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = player_->getEndstoneActor<EndstonePlayer>();
    endstone::PlayerInteractActorEvent e{player, actor.getEndstoneActor()};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&GameMode::interact, this, actor, location);
}
