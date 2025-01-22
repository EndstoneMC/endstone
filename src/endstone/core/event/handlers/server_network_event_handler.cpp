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

#include "endstone/core/event/handlers/server_network_event_handler.h"

#include <variant>

#include "endstone/core/server.h"
#include "endstone/event/player/player_chat_event.h"

namespace endstone::core {

EndstoneServerNetworkEventHandler::EndstoneServerNetworkEventHandler(std::unique_ptr<ServerNetworkEventHandler> handle)
    : handle_(std::move(handle))
{
}

GameplayHandlerResult<CoordinatorResult> EndstoneServerNetworkEventHandler::handleEvent(
    MutableServerNetworkGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<ChatEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

bool EndstoneServerNetworkEventHandler::handleEvent(ChatEvent &event)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        PlayerChatEvent e{player->getEndstoneActor<EndstonePlayer>(), event.message};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        event.message = std::move(e.getMessage());
        server.getLogger().info("<{}> {}", e.getPlayer().getName(), e.getMessage());
    }
    return true;
}

}  // namespace endstone::core
