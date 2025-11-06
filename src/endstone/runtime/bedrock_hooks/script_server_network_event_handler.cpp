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

#include "bedrock/network/packet/text_packet.h"
#include "endstone/core/player.h"
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

GameplayHandlerResult<CoordinatorResult> handleEvent(ChatEvent &event,
                                                     std::function<GameplayHandlerResult<CoordinatorResult>()> original)
{
    auto *player = WeakEntityRef(event.sender).tryUnwrap<::Player>();
    if (!player) {
        return original();
    }

    static GameplayHandlerResult CANCEL = {HandlerResult::BypassListeners, CoordinatorResult::Cancel};

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
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

    // call endstone's chat event before the script api
    endstone::PlayerChatEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(), event.message, recipients};
    auto original_format = e.getFormat();

    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return CANCEL;
    }

    // check if the format is valid
    std::string message;
    try {
        message = fmt::format(fmt::runtime(e.getFormat()), e.getPlayer().getName(), e.getMessage());
    }
    catch (const std::exception & /*e*/) {
        server.getLogger().error("Invalid format string encountered in PlayerChatEvent.");
        return CANCEL;
    }

    // call scripting api chat events
    event.message = e.getMessage();
    auto result = original();
    if (result.return_value != CoordinatorResult::Continue) {
        return result;
    }

    // Format has been changed, send formatted messages as raw and cancel the original event to avoid double sending
    if (e.getFormat() != original_format) {
        for (const auto &recipient : e.getRecipients()) {
            recipient->sendMessage(message);
        }
        server.getLogger().info(message);
        return CANCEL;
    }

    // Message has been changed, send new text messages and cancel the original event to avoid double sending
    if (e.getMessage() != event.message) {
        player = &static_cast<endstone::core::EndstonePlayer &>(e.getPlayer()).getHandle();
        for (const auto &recipient : e.getRecipients()) {
            auto packet = TextPacket::createChat(player->getName(), e.getMessage(), std::nullopt, player->getXuid(),
                                                 player->getPlatformOnlineId());
            static_cast<endstone::core::EndstonePlayer *>(recipient)->getHandle().sendNetworkPacket(packet);
        }
        return CANCEL;
    }

    // Nothing changed, proceed to original route and log to console
    server.getLogger().info(message);
    return result;
}
}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptServerNetworkEventHandler::handleEvent1(
    MutableServerNetworkGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<ChatEvent>>) {
            return handleEvent(arg.value(), [&]() {
                return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptServerNetworkEventHandler::handleEvent1, this, event);
            });
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptServerNetworkEventHandler::handleEvent1, this, event);
    };
    return event.visit(visitor);
}
