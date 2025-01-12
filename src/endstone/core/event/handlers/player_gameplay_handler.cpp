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
#include "endstone/core/json.h"
#include "endstone/core/player.h"

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
    return handle_->handleEvent(event);
}

GameplayHandlerResult<CoordinatorResult> EndstonePlayerGameplayHandler::handleEvent(
    MutablePlayerGameplayEvent<CoordinatorResult> &event)
{
    return handle_->handleEvent(event);
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormResponseEvent &event)
{
    const StackResultStorageEntity stack_result(event.player);
    if (const auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor) {
        actor->getEndstoneActor<EndstonePlayer>().onFormResponse(event.form_id, event.form_response);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormCloseEvent &event)
{
    const StackResultStorageEntity stack_result(event.player);
    if (const auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor) {
        actor->getEndstoneActor<EndstonePlayer>().onFormClose(event.form_id, event.form_close_reason);
    }
    return true;
}

}  // namespace endstone::core
