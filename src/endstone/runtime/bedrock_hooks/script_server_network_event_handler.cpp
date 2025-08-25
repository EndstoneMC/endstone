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

#include "bedrock/scripting/event_handlers/script_server_network_event_handler.h"

#include "endstone/core/server.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/server/packet_receive_event.h"
#include "endstone/event/server/packet_send_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(IncomingPacketEvent & /*event*/)
{
    return true;
}

bool handleEvent(OutgoingPacketEvent & /*event*/)
{
    return true;
}

bool handleEvent(ChatEvent &event)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    if (auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>(); player) {
        // populate recipient list
        std::optional<std::vector<endstone::Player *>> recipients = std::nullopt;
        if (event.targets.has_value()) {
            auto r = std::vector<endstone::Player *>();
            for (const auto &weak_ref : event.targets.value()) {
                if (const auto *receipt = WeakEntityRef(weak_ref).tryUnwrap<::Player>(); receipt) {
                    r.emplace_back(&receipt->getEndstoneActor<endstone::core::EndstonePlayer>());
                }
            }
            recipients = r;
        }

        // create chat event
        endstone::PlayerChatEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), event.message,
                                    recipients};
        auto original_format = e.getFormat();
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }

        std::string message;
        try {
            message = fmt::format(fmt::runtime(e.getFormat()), e.getPlayer().getName(), e.getMessage());
        }
        catch (const std::exception & /*e*/) {
            server.getLogger().error("Invalid format string encountered in PlayerChatEvent.");
            return false;
        }

        // if format's changed, send as normal message instead of text message
        if (e.getFormat() != original_format) {
            for (const auto &recipient : e.getRecipients()) {
                recipient->sendMessage(message);
            }
            server.getLogger().info(message);
            return false;
        }

        event.author = e.getPlayer().getName();
        event.message = std::move(e.getMessage());
        server.getLogger().info(message);
    }
    return true;
}
}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptServerNetworkEventHandler::handleEvent1(
    MutableServerNetworkGameplayEvent<CoordinatorResult> &event)
{
    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptServerNetworkEventHandler::handleEvent1, this, event);
    if (result.handler_result == HandlerResult::BypassListeners || result.return_value == CoordinatorResult::Cancel) {
        return result;
    }

    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if (!handleEvent(arg.value())) {
            return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
        }
        return result;
    };
    return event.visit(visitor);
}
