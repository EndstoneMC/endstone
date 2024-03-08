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

#include "bedrock/server/server_instance.h"

#include <pybind11/pybind11.h>

#include "endstone/detail/hook.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/util/color_format.h"

namespace py = pybind11;

using endstone::ColorFormat;
using endstone::detail::EndstoneServer;
using endstone::detail::PythonPluginLoader;
using endstone::detail::Singleton;

void ServerInstance::startServerThread()
{
    Singleton<EndstoneServer>::setInstance(std::make_unique<EndstoneServer>(*this));
    auto &server = Singleton<EndstoneServer>::getInstance();
    server.getPluginManager().registerLoader(std::make_unique<PythonPluginLoader>(server));
    server.getLogger().info(ColorFormat::DARK_AQUA + ColorFormat::BOLD +
                                "This server is running {} version: {} (Minecraft: {})",
                            server.getName(), server.getVersion(), server.getMinecraftVersion());
    server.loadPlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstance::startServerThread, this);
}

Minecraft &ServerInstance::getMinecraft()
{
#ifdef __linux__
    return **reinterpret_cast<Minecraft **>(reinterpret_cast<size_t *>(this) + 16);
#elif _WIN32
    return **reinterpret_cast<Minecraft **>(reinterpret_cast<size_t *>(this) + 21);
#endif
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    Singleton<EndstoneServer>::getInstance().enablePlugins();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStarted, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance &instance)
{
    py::gil_scoped_acquire acquire{};
    Singleton<EndstoneServer>::getInstance().disablePlugins();
    Singleton<EndstoneServer>::reset();  // we need to explicitly acquire GIL and destroy the server instance as the
                                         // command map and the plugin manager hold shared_ptrs to python objects
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerThreadStopped, this, instance);
}
