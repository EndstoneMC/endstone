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

#include "endstone/detail/hook.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"
#include "endstone/event/server/script_message_event.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

CoordinatorResult ScriptingEventCoordinator::sendEvent(EventRef<ScriptingGameplayEvent<CoordinatorResult>> ref)
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &level = static_cast<EndstoneLevel *>(server.getLevel())->getHandle();

    return std::visit(endstone::overloaded{[&](const Details::ValueOrRef<ScriptCommandMessageEvent const> &arg) {
                          const auto &event = arg.value();
                          endstone::CommandSender *sender = nullptr;
                          if (event.source_actor.has_value()) {
                              if (auto *actor = level.fetchEntity(event.source_actor.value(), false); actor) {
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
                          return ENDSTONE_HOOK_CALL_ORIGINAL(&ScriptingEventCoordinator::sendEvent, this, event);
                      }},
                      ref.get().variant);
}
