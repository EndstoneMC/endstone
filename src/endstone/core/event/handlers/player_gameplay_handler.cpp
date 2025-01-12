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

#include "endstone/core/event/handlers/player_gameplay_handler.h"

#include "bedrock/world/actor/actor.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/json.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"

namespace endstone::core {

EndstonePlayerGameplayHandler::EndstonePlayerGameplayHandler(std::unique_ptr<PlayerGameplayHandler> handle)
    : handle_(std::move(handle))
{
}

HandlerResult EndstonePlayerGameplayHandler::handleEvent(const PlayerGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const PlayerFormResponseEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerFormCloseEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

GameplayHandlerResult<CoordinatorResult> EndstonePlayerGameplayHandler::handleEvent(
    const PlayerGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const PlayerInteractWithBlockBeforeEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerInteractWithEntityBeforeEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

GameplayHandlerResult<CoordinatorResult> EndstonePlayerGameplayHandler::handleEvent(
    MutablePlayerGameplayEvent<CoordinatorResult> &event)
{
    return handle_->handleEvent(event);
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormResponseEvent &event)
{
    const StackResultStorageEntity stack_result(event.player);
    if (const auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor && actor->isPlayer()) {
        actor->getEndstoneActor<EndstonePlayer>().onFormResponse(event.form_id, event.form_response);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormCloseEvent &event)
{
    const StackResultStorageEntity stack_result(event.player);
    if (const auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor && actor->isPlayer()) {
        actor->getEndstoneActor<EndstonePlayer>().onFormClose(event.form_id, event.form_close_reason);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerInteractWithBlockBeforeEvent &event)
{
    const StackResultStorageEntity stack_result(event.player);
    if (const auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor && actor->isPlayer()) {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto &player = actor->getEndstoneActor<EndstonePlayer>();
        auto &block_source = player.getHandle().getDimension().getBlockSourceFromMainChunkSource();
        if (auto block = EndstoneBlock::at(block_source, BlockPos(event.block_location))) {
            const std::shared_ptr<EndstoneItemStack> item_stack =
                event.item.isNull() ? nullptr : EndstoneItemStack::fromMinecraft(event.item);

            PlayerInteractEvent e{
                player,
                item_stack,
                block.value(),
                static_cast<BlockFace>(event.block_face),
                {event.face_location.x, event.face_location.y, event.face_location.z},
            };
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

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerInteractWithEntityBeforeEvent &event)
{
    const StackResultStorageEntity stack_result_player(event.player);
    const auto *player = ::Actor::tryGetFromEntity(stack_result_player.getStackRef(), false);

    const StackResultStorageEntity stack_result_target(event.target_entity);
    const auto *target = ::Actor::tryGetFromEntity(stack_result_target.getStackRef(), false);

    if (player && player->isPlayer() && target) {
        const auto &server = entt::locator<EndstoneServer>::value();
        PlayerInteractActorEvent e{player->getEndstoneActor<EndstonePlayer>(), target->getEndstoneActor()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

}  // namespace endstone::core
