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

#include "bedrock/scripting/event_handlers/script_block_gameplay_handler.h"

#include <optional>

#include "bedrock/core/math/vec3.h"
#include "bedrock/world/actor/actor.h"
#include "endstone/block/block_face.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/player.h"
#include "endstone/event/actor/actor_explode_event.h"
#include "endstone/event/block/block_break_event.h"
#include "endstone/event/block/block_explode_event.h"
#include "endstone/event/block/block_piston_event.h"
#include "endstone/event/block/block_place_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace endstone::runtime {
const std::optional<Vec3> &getLastExplosionPos();
}  // namespace endstone::runtime

namespace {

bool handleEvent(const PistonActionEvent &event)
{
    // TODO(event): piston events
    return true;
}

bool handleEvent(const BlockTryPlaceByPlayerEvent &event)
{
    const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>();
    if (!player) {
        return true;
    }

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &endstone_player = player->getEndstoneActor<endstone::core::EndstonePlayer>();
    auto &dimension = endstone_player.getDimension();
    auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
    const auto block_face = static_cast<endstone::BlockFace>(event.face);
    auto block_placed =
        std::make_unique<endstone::core::EndstoneBlockState>(dimension, event.pos, event.permutation_to_place);
    auto block_replaced = endstone::core::EndstoneBlock::at(block_source, event.pos);
    const auto opposite = endstone::core::EndstoneBlockFace::getOpposite(block_face);
    auto block_against = block_replaced->getRelative(opposite);

    endstone::BlockPlaceEvent e{std::move(block_placed), std::move(block_replaced), std::move(block_against),
                                endstone_player};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return true;
}

bool handleEvent(ExplosionStartedEvent &event)
{
    const auto *source = WeakEntityRef(event.source).tryUnwrap<::Actor>();
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();

    std::vector<std::unique_ptr<endstone::Block>> block_list;
    for (const auto &pos : event.blocks) {
        block_list.emplace_back(
            endstone::core::EndstoneBlock::at(event.dimension.getBlockSourceFromMainChunkSource(), pos));
    }

    if (source) {
        auto &actor = source->getEndstoneActor<>();
        endstone::ActorExplodeEvent e{actor, actor.getLocation(), std::move(block_list)};
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
        auto &block_source = event.dimension.getBlockSourceFromMainChunkSource();
        const auto &pos = endstone::runtime::getLastExplosionPos();
        if (!pos) {
            server.getLogger().critical("BlockExplodeEvent: invalid explosion position");
            return true;
        }
        endstone::BlockExplodeEvent e{endstone::core::EndstoneBlock::at(block_source, BlockPos(pos.value())),
                                      std::move(block_list)};
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
    return true;
}

bool handleEvent(BlockTryDestroyByPlayerEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
        auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
        if (player->getPlayerGameType() == GameType::Creative) {
            if (!event.item_used.isNull() && !event.item_used.getItem()->canDestroyInCreative()) {
                return false;
            }
        }

        endstone::BlockBreakEvent e{endstone::core::EndstoneBlock::at(block_source, event.pos),
                                    player->getEndstoneActor<endstone::core::EndstonePlayer>()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
        // update here as plugins may change the player inventory during the event handling
        event.item_used = player->getInventory().getItem(player->getSelectedItemSlot());
    }
    return true;
}
}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptBlockGameplayHandler::handleEvent2(
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
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptBlockGameplayHandler::handleEvent2, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptBlockGameplayHandler::handleEvent4(
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
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptBlockGameplayHandler::handleEvent4, this, event);
    };
    return event.visit(visitor);
}
