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

#include "endstone/core/event/handlers/actor_gameplay_handler.h"

#include "bedrock/world/actor/actor.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_death_event.h"

namespace endstone::core {

EndstoneActorGameplayHandler::EndstoneActorGameplayHandler(std::unique_ptr<ActorGameplayHandler> handle)
    : handle_(std::move(handle))
{
}

HandlerResult EndstoneActorGameplayHandler::handleEvent(const ActorGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const ActorKilledEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

GameplayHandlerResult<CoordinatorResult> EndstoneActorGameplayHandler::handleEvent(
    const ActorGameplayEvent<CoordinatorResult> &event)
{
    return handle_->handleEvent(event);
}

GameplayHandlerResult<CoordinatorResult> EndstoneActorGameplayHandler::handleEvent(
    MutableActorGameplayEvent<CoordinatorResult> &event)
{
    return handle_->handleEvent(event);
}

bool EndstoneActorGameplayHandler::handleEvent(const ActorKilledEvent &event)
{
    if (const auto *mob = WeakEntityRef(event.actor_context).tryUnwrap<::Mob>(); mob && !mob->isPlayer()) {
        const auto &server = entt::locator<EndstoneServer>::value();
        ActorDeathEvent e{mob->getEndstoneActor<EndstoneMob>()};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

}  // namespace endstone::core
