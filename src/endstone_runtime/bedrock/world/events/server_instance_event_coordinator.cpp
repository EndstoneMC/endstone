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
#include "endstone/detail/hook.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"
#include "endstone/detail/signal_handler.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin_load_order.h"

namespace py = pybind11;

using endstone::PluginLoadOrder;
using endstone::ServerLoadEvent;
using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneScoreboard;
using endstone::detail::EndstoneServer;
using endstone::detail::PythonPluginLoader;

void ServerInstanceEventCoordinator::sendServerInitializeStart(ServerInstance &instance)
{
    endstone::detail::register_signal_handler();
    auto &server = entt::locator<EndstoneServer>::value_or();
    server.init(instance);
    server.loadPlugins();
    server.enablePlugins(PluginLoadOrder::Startup);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerInitializeStart, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &level = *instance.getMinecraft().getLevel();
    server.setLevel(std::make_unique<EndstoneLevel>(level));
    server.setScoreboard(std::make_unique<EndstoneScoreboard>(level.getScoreboard()));
    server.setCommandMap(std::make_unique<endstone::detail::EndstoneCommandMap>(server));
    server.enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Startup};
    server.getPluginManager().callEvent(event);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    py::gil_scoped_acquire acquire{};
    entt::locator<EndstoneServer>::value().disablePlugins();
    entt::locator<EndstoneServer>::reset();  // we explicitly acquire GIL and destroy the server instance as the command
                                             // map and the plugin manager hold shared_ptrs to python objects
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance);
}
