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

#include "bedrock/world/events/server_instance_event_coordinator.h"

#include <pybind11/pybind11.h>

#include "bedrock/server/server_instance.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/level/level.h"
#include "endstone/core/scoreboard/scoreboard.h"
#include "endstone/core/server.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin_load_order.h"
#include "endstone/runtime/hook.h"

namespace py = pybind11;

using endstone::PluginLoadOrder;
using endstone::ServerLoadEvent;
using endstone::core::EndstoneLevel;
using endstone::core::EndstoneScoreboard;
using endstone::core::EndstoneServer;

void ServerInstanceEventCoordinator::sendServerInitializeStart(ServerInstance &instance)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerInitializeStart, this, instance);
    auto &server = entt::locator<EndstoneServer>::value_or();
    server.init(instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance);
    auto &server = entt::locator<EndstoneServer>::value();
    server.setLevel(*instance.getMinecraft()->getLevel());
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    entt::locator<EndstoneServer>::reset();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance);
}
