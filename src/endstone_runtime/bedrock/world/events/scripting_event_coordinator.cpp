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
#include "endstone/detail/server.h"

using endstone::detail::EndstoneServer;

CoordinatorResult ScriptingEventCoordinator::sendEvent(EventRef<ScriptingGameplayEvent<CoordinatorResult>> ref)
{
    auto &server = entt::locator<EndstoneServer>::value();
    return std::visit(endstone::overloaded{[&](const Details::ValueOrRef<ScriptCommandMessageEvent const> &arg) {
                          // TODO(event): call endstone::ScriptEventCommandEvent
                          const auto &event = arg.value();

                          // fix: wtf mojang devs - the original function accesses the pointer without checking
                          if (!scripting_event_handler_) {
                              return CoordinatorResult::Continue;
                          }
                          return ENDSTONE_HOOK_CALL_ORIGINAL(&ScriptingEventCoordinator::sendEvent, this, event);
                      }},
                      ref.get().variant);
}
