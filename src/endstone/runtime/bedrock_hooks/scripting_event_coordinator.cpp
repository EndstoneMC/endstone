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

#include "bedrock/world/events/scripting_event_coordinator.h"

#include <entt/entt.hpp>

#include "bedrock/gameplayhandlers/coordinator_result.h"
#include "bedrock/world/events/event_variant.h"
#include "bedrock/world/events/scripting_events.h"
#include "endstone/core/server.h"
#include "endstone/event/server/script_message_event.h"
#include "endstone/runtime/hook.h"

using endstone::core::EndstoneServer;

CoordinatorResult ScriptingEventCoordinator::sendEvent(EventRef<ScriptingGameplayEvent<CoordinatorResult>> ref)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    constexpr auto symbol = __FUNCDNAME__;
    auto visitor = [&](auto &&arg) -> CoordinatorResult {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Details::ValueOrRef<const ScriptCommandMessageEvent>>) {
            const auto &event = arg.value();
            const endstone::CommandSender *sender = nullptr;
            if (event.source_actor.has_value()) {
                if (auto *actor = event.level.fetchEntity(event.source_actor.value(), false); actor) {
                    sender = &actor->getEndstoneActor();
                }
            }
            // TODO(command): add support for BlockCommandSender
            if (!sender) {
                sender = &server.getCommandSender();
            }
            endstone::ScriptMessageEvent e{event.message_id, event.message_value, *sender};
            server.getPluginManager().callEvent(e);

            // fix: wtf mojang devs - the original function accesses the pointer without checking
            if (!scripting_event_handler_) {
                return CoordinatorResult::Continue;
            }
        }

        const auto &event = arg.value();
        return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ScriptingEventCoordinator::sendEvent, symbol, this, event);
    };

    return std::visit(visitor, ref.get().variant);
}
