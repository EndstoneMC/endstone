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

#include "endstone/core/event/handlers/block_gameplay_handler.h"

#include <magic_enum/magic_enum.hpp>

#include "bedrock/world/actor/actor.h"
#include "endstone/block/block_face.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/player.h"
#include "endstone/event/actor/actor_explode_event.h"
#include "endstone/event/block/block_break_event.h"
#include "endstone/event/block/block_piston_event.h"
#include "endstone/event/block/block_place_event.h"

namespace endstone::core {

EndstoneBlockGameplayHandler::EndstoneBlockGameplayHandler(std::unique_ptr<BlockGameplayHandler> handle)
    : handle_(std::move(handle))
{
}

HandlerResult EndstoneBlockGameplayHandler::handleEvent(const BlockGameplayEvent<void> &event)
{
    return handle_->handleEvent(event);
}

GameplayHandlerResult<CoordinatorResult> EndstoneBlockGameplayHandler::handleEvent(
    const BlockGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const BlockTryPlaceByPlayerEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PistonActionEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

GameplayHandlerResult<std::optional<std::string>> EndstoneBlockGameplayHandler::handleEvent(
    const BlockGameplayEvent<std::optional<std::string>> &event)
{
    return handle_->handleEvent(event);
}

GameplayHandlerResult<CoordinatorResult> EndstoneBlockGameplayHandler::handleEvent(
    MutableBlockGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<ExplosionStartedEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<BlockTryDestroyByPlayerEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

bool EndstoneBlockGameplayHandler::handleEvent(const PistonActionEvent &event)
{
    // TODO(event): piston events
    return true;
}

bool EndstoneBlockGameplayHandler::handleEvent(const BlockTryPlaceByPlayerEvent &event)
{
    const auto *entity = ::Actor::tryGetFromEntity(event.player.unwrap(), false);
    if (!entity || !entity->isPlayer()) {
        return true;
    }

    const auto &server = entt::locator<EndstoneServer>::value();
    auto &player = entity->getEndstoneActor<EndstonePlayer>();
    auto &dimension = player.getDimension();
    auto &block_source = player.getHandle().getDimension().getBlockSourceFromMainChunkSource();
    const auto block_face = static_cast<BlockFace>(event.face);
    auto block_placed = std::make_unique<EndstoneBlockState>(dimension, event.pos, event.permutation_to_place);
    if (const auto block_replaced = EndstoneBlock::at(block_source, event.pos)) {
        const auto opposite = EndstoneBlockFace::getOpposite(block_face);
        if (const auto block_against = block_replaced.value()->getRelative(opposite)) {
            BlockPlaceEvent e{std::move(block_placed), block_replaced.value(), block_against.value(), player};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                return false;
            }
        }
        else {
            server.getLogger().error(block_against.error());
        }
    }
    else {
        server.getLogger().error(block_replaced.error());
    }
    return true;
}

bool EndstoneBlockGameplayHandler::handleEvent(ExplosionStartedEvent &event)
{
    const auto *source = WeakEntityRef(event.source).tryUnwrap<::Actor>();
    const auto &server = entt::locator<EndstoneServer>::value();

    std::vector<std::shared_ptr<Block>> block_list;
    for (const auto &pos : event.blocks) {
        if (auto block_or_error = EndstoneBlock::at(event.dimension.getBlockSourceFromMainChunkSource(), pos)) {
            block_list.emplace_back(block_or_error.value());
        }
        else {
            server.getLogger().error(block_or_error.error());
            return true;
        }
    }

    if (source) {
        auto &actor = source->getEndstoneActor<>();
        ActorExplodeEvent e{actor, actor.getLocation(), block_list};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
        event.blocks.clear();
        for (const auto &block : e.getBlockList()) {
            if (block) {
                event.blocks.emplace(block->getX(), block->getY(), block->getZ());
            }
        }
    }
    else {
        // TODO(event): BlockExplodeEvent
    }
    return true;
}

bool EndstoneBlockGameplayHandler::handleEvent(BlockTryDestroyByPlayerEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
        if (const auto block = EndstoneBlock::at(block_source, event.pos)) {
            BlockBreakEvent e{block.value(), player->getEndstoneActor<EndstonePlayer>()};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                return false;
            }
        }
        else {
            server.getLogger().error(block.error());
        }
    }
    return true;
}

}  // namespace endstone::core
