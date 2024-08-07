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

#include "bedrock/world/events/event_coordinator.h"

#include <sstream>

#include <bedrock/world/scores/server_scoreboard.h>
#include <entt/entt.hpp>
#include <pybind11/pybind11.h>
#include <spdlog/spdlog.h>

#include "bedrock/deps/jsoncpp/nlohmann_json.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/world/events/coordinator_result.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/detail/scoreboard/scoreboard.h"
#include "endstone/detail/server.h"
#include "endstone/detail/signal_handler.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin_load_order.h"

namespace py = pybind11;

using endstone::ColorFormat;
using endstone::PluginLoadOrder;
using endstone::ServerLoadEvent;
using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneScoreboard;
using endstone::detail::EndstoneServer;
using endstone::detail::PythonPluginLoader;

// void ActorEventCoordinator::sendEvent(const EventRef<ActorGameplayEvent<void>> &ref)
//{
//     void (ActorEventCoordinator::*fp)(const EventRef<ActorGameplayEvent<void>> &) =
//     &ActorEventCoordinator::sendEvent; auto visitor = entt::overloaded{
//         // [](Details::ValueOrRef<ActorRemovedEvent const> value) { cpptrace::generate_trace().print(); },
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
// }
//
// CoordinatorResult ActorEventCoordinator::sendEvent(const EventRef<ActorGameplayEvent<CoordinatorResult>> &ref)
//{
//     CoordinatorResult (ActorEventCoordinator::*fp)(const EventRef<ActorGameplayEvent<CoordinatorResult>> &) =
//         &ActorEventCoordinator::sendEvent;
//     auto visitor = entt::overloaded{
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
// }
//
// CoordinatorResult BlockEventCoordinator::sendEvent(const EventRef<BlockGameplayEvent<CoordinatorResult>> &ref)
//{
//     CoordinatorResult (BlockEventCoordinator::*fp)(const EventRef<BlockGameplayEvent<CoordinatorResult>> &) =
//         &BlockEventCoordinator::sendEvent;
//     auto visitor = entt::overloaded{
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
// }
//
// void BlockEventCoordinator::sendEvent(const EventRef<BlockGameplayEvent<void>> &ref)
//{
//     void (BlockEventCoordinator::*fp)(const EventRef<BlockGameplayEvent<void>> &) =
//     &BlockEventCoordinator::sendEvent; auto visitor = entt::overloaded{
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
// }

// void LevelEventCoordinator::sendEvent(const EventRef<LevelGameplayEvent<void>> &ref)
//{
//     auto visitor = entt::overloaded{
//         // [](Details::ValueOrRef<LevelAddedActorEvent const> value) { cpptrace::generate_trace().print(); },
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     ENDSTONE_HOOK_CALL_ORIGINAL(&LevelEventCoordinator::sendEvent, this, ref);
// }

LevelGameplayHandler &LevelEventCoordinator::getLevelGameplayHandler()
{
    return *level_gameplay_handler_;
}

void PlayerEventCoordinator::sendEvent(const EventRef<PlayerGameplayEvent<void>> &ref)
{
    void (PlayerEventCoordinator::*fp)(const EventRef<PlayerGameplayEvent<void>> &) =
        &PlayerEventCoordinator::sendEvent;
    auto visitor = entt::overloaded{
        [](Details::ValueOrRef<PlayerFormCloseEvent const> value) {
            const auto &event = value.value();
            const auto &weak_ref = event.player;
            EntityContext ctx{*weak_ref.storage.registry, weak_ref.storage.entity_id};
            auto *player = static_cast<Player *>(Actor::tryGetFromEntity(ctx, false));
            if (player) {
                // Players can be null if they are dead when we receive the event
                player->getEndstonePlayer().onFormClose(event.form_id, event.form_close_reason);
            }
        },
        [](Details::ValueOrRef<PlayerFormResponseEvent const> value) {
            const auto &event = value.value();
            const auto &weak_ref = event.player;
            EntityContext ctx{*weak_ref.storage.registry, weak_ref.storage.entity_id};
            auto *player = static_cast<Player *>(Actor::tryGetFromEntity(ctx, false));
            if (player) {
                // Players can be null if they are dead when we receive the event
                player->getEndstonePlayer().onFormResponse(event.form_id, Json::to_nlohmann(event.form_response));
            }
        },
        [](auto &&ignored) {},
    };
    std::visit(visitor, ref.variant.variant.variant);
    ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
}

// CoordinatorResult PlayerEventCoordinator::sendEvent(const EventRef<PlayerGameplayEvent<CoordinatorResult>> &ref)
//{
//     CoordinatorResult (PlayerEventCoordinator::*fp)(const EventRef<PlayerGameplayEvent<CoordinatorResult>> &) =
//         &PlayerEventCoordinator::sendEvent;
//     auto visitor = entt::overloaded{
//         [](auto ignored) {},
//     };
//     std::visit(visitor, ref.variant.event);
//     return ENDSTONE_HOOK_CALL_ORIGINAL(fp, this, ref);
// }

void ServerInstanceEventCoordinator::sendServerInitializeStart(ServerInstance &instance)
{
    endstone::detail::register_signal_handler();
    auto &server = entt::locator<EndstoneServer>::value_or(instance);
    server.init();
    server.loadPlugins();
    server.enablePlugins(PluginLoadOrder::Startup);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerInitializeStart, this, instance);
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance &instance)
{
    auto &server = entt::locator<EndstoneServer>::value();
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

void ServerInstanceEventCoordinator::sendServerLevelInitialized(ServerInstance &instance, Level &level)
{
    auto &server = entt::locator<EndstoneServer>::value();
    server.setLevel(std::make_unique<EndstoneLevel>(level));
    server.setScoreboard(std::make_unique<EndstoneScoreboard>(level.getScoreboard()));
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerInstanceEventCoordinator::sendServerLevelInitialized, this, instance, level);
}
