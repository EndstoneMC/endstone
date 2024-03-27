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

#include "bedrock/event/event_coordinator.h"

#include <iomanip>
#include <sstream>

#include <pybind11/pybind11.h>
#include <spdlog/spdlog.h>

#include "bedrock/server/server_instance.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/util/color_format.h"

namespace py = pybind11;

using endstone::ColorFormat;
using endstone::PluginLoadOrder;
using endstone::ServerLoadEvent;
using endstone::detail::EndstoneServer;
using endstone::detail::PythonPluginLoader;
using endstone::detail::Singleton;

namespace {
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template <typename T>
std::string hexDump(T &obj)
{
    const auto *ptr = reinterpret_cast<const unsigned char *>(&obj);
    std::ostringstream oss;
    for (std::size_t i = 0; i < sizeof(obj) + 16; ++i) {
        if (i == sizeof(obj)) {
            oss << " | ";
        }

        if (i % 16 == 0) {
            oss << reinterpret_cast<const void *>(ptr + i) << ": ";
        }
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(ptr[i]) << " ";
        if (i % 16 == 15) {
            oss << "\n";
        }
    }
    oss << "\n";
    return oss.str();
}

}  // namespace

void ActorEventCoordinator::sendEvent(const EventRef<ActorGameplayEvent<void>> &ref)
{
    void (ActorEventCoordinator::*fp)(const EventRef<ActorGameplayEvent<void>> &) = &ActorEventCoordinator::sendEvent;
    std::visit(
        [](auto &&arg) {
            // TODO:
        },
        ref.reference.event);

    ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
}

CoordinatorResult ActorEventCoordinator::sendEvent(const EventRef<ActorGameplayEvent<CoordinatorResult>> &ref)
{
    CoordinatorResult (ActorEventCoordinator::*fp)(const EventRef<ActorGameplayEvent<CoordinatorResult>> &) =
        &ActorEventCoordinator::sendEvent;
    std::visit(
        [](auto &&arg) {
            // TODO:
        },
        ref.reference.event);

    return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
}

void LevelEventCoordinator::sendEvent(const EventRef<LevelGameplayEvent<void>> &ref)
{
    std::visit(
        [](auto &&arg) {
            // TODO:
        },
        ref.reference.event);

    ENDSTONE_HOOK_CALL_ORIGINAL(&LevelEventCoordinator::sendEvent, this, ref);
}

void PlayerEventCoordinator::sendEvent(const EventRef<PlayerGameplayEvent<void>> &ref)
{
    void (PlayerEventCoordinator::*fp)(const EventRef<PlayerGameplayEvent<void>> &) =
        &PlayerEventCoordinator::sendEvent;
    std::visit(
        [](auto &&arg) {
            // TODO:
        },
        ref.reference.event);
    ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
}

CoordinatorResult PlayerEventCoordinator::sendEvent(const EventRef<PlayerGameplayEvent<CoordinatorResult>> &ref)
{
    CoordinatorResult (PlayerEventCoordinator::*fp)(const EventRef<PlayerGameplayEvent<CoordinatorResult>> &) =
        &PlayerEventCoordinator::sendEvent;
    std::visit(
        [](auto &&arg) {
            // TODO:
        },
        ref.reference.event);
    return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
}

void ServerInstanceEventCoordinator::sendServerInitializeStart(ServerInstance &instance)
{
    Singleton<EndstoneServer>::setInstance(std::make_unique<EndstoneServer>(instance));
    auto &server = Singleton<EndstoneServer>::getInstance();
    server.getPluginManager().registerLoader(std::make_unique<PythonPluginLoader>(server));
    server.getLogger().info(ColorFormat::DARK_AQUA + ColorFormat::BOLD +
                                "This server is running {} version: {} (Minecraft: {})",
                            server.getName(), server.getVersion(), server.getMinecraftVersion());

    server.loadPlugins();
    server.enablePlugins(PluginLoadOrder::Startup);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerInitializeStart, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    auto &server = Singleton<EndstoneServer>::getInstance();
    server.enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Startup};
    server.getPluginManager().callEvent(event);
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
