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

#include "bedrock/scripting/server_script_manager.h"

#include <mutex>
#include <thread>

#include "bedrock/scripting/event_handlers/script_actor_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_block_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_item_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_level_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_player_gameplay_handler.h"
#include "bedrock/scripting/event_handlers/script_scripting_event_handler.h"
#include "bedrock/scripting/event_handlers/script_server_network_event_handler.h"
#include "endstone/core/server.h"
#include "endstone/detail.h"
#include "endstone/runtime/hook.h"
#include "endstone/runtime/vtable_hook.h"

namespace vhook = endstone::runtime::vhook;

template <typename T>
void hookEventHandler(T &handler);

template <>
void hookEventHandler(ActorGameplayHandler &handler)
{
#ifdef _WIN32
    vhook::create<4>(&handler, &ScriptActorGameplayHandler::handleEvent1);
    vhook::create<1>(&handler, &ScriptBlockGameplayHandler::handleEvent4);
#else
    vhook::create<2>(&handler, &ScriptActorGameplayHandler::handleEvent1);
    vhook::create<5>(&handler, &ScriptBlockGameplayHandler::handleEvent4);
#endif
}

template <>
void hookEventHandler(BlockGameplayHandler &handler)
{
#ifdef _WIN32
    vhook::create<3>(&handler, &ScriptBlockGameplayHandler::handleEvent2);
    vhook::create<1>(&handler, &ScriptBlockGameplayHandler::handleEvent4);
#else
    vhook::create<3>(&handler, &ScriptBlockGameplayHandler::handleEvent2);
    vhook::create<5>(&handler, &ScriptBlockGameplayHandler::handleEvent4);
#endif
}

template <>
void hookEventHandler(ItemGameplayHandler &handler)
{
#ifdef _WIN32
    // vhook::create<2>(&handler, &ScriptItemGameplayHandler::handleEvent1);
    vhook::create<1>(&handler, &ScriptItemGameplayHandler::handleEvent2);
#else
    // vhook::create<2>(&handler, &ScriptItemGameplayHandler::handleEvent1);
    vhook::create<3>(&handler, &ScriptItemGameplayHandler::handleEvent2);
#endif
}

template <>
void hookEventHandler(LevelGameplayHandler &handler)
{
#ifdef _WIN32
    vhook::create<2>(&handler, &ScriptLevelGameplayHandler::handleEvent1);
    vhook::create<1>(&handler, &ScriptLevelGameplayHandler::handleEvent2);
#else
    vhook::create<2>(&handler, &ScriptLevelGameplayHandler::handleEvent1);
    vhook::create<3>(&handler, &ScriptLevelGameplayHandler::handleEvent2);
#endif
}

template <>
void hookEventHandler(PlayerGameplayHandler &handler)
{
#ifdef _WIN32
    vhook::create<3>(&handler, &ScriptPlayerGameplayHandler::handleEvent1);
    vhook::create<2>(&handler, &ScriptPlayerGameplayHandler::handleEvent2);
    vhook::create<1>(&handler, &ScriptPlayerGameplayHandler::handleEvent3);
#else
    vhook::create<2>(&handler, &ScriptPlayerGameplayHandler::handleEvent1);
    vhook::create<3>(&handler, &ScriptPlayerGameplayHandler::handleEvent2);
    vhook::create<4>(&handler, &ScriptPlayerGameplayHandler::handleEvent3);
#endif
}

template <>
void hookEventHandler(ScriptingEventHandler &handler)
{
#ifdef _WIN32
    vhook::create<1>(&handler, &ScriptScriptingEventHandler::handleEvent2);
#else
    vhook::create<3>(&handler, &ScriptScriptingEventHandler::handleEvent2);
#endif
}

template <>
void hookEventHandler(ServerNetworkEventHandler &handler)
{
#ifdef _WIN32
    vhook::create<1>(&handler, &ScriptServerNetworkEventHandler::handleEvent1);
#else
    vhook::create<2>(&handler, &ScriptServerNetworkEventHandler::handleEvent1);
#endif
}

void ServerScriptManager::_runPlugins(PluginExecutionGroup exe_group, ServerInstance &server_instance)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerScriptManager::_runPlugins, this, exe_group, server_instance);
    static std::once_flag init_server, init_level;
    switch (exe_group) {
    case PluginExecutionGroup::PrePackLoadExecution: {
        std::call_once(init_server, [&server_instance]() {
            auto &server = entt::locator<endstone::core::EndstoneServer>::value_or();
            server.init(server_instance);
        });
        break;
    }
    case PluginExecutionGroup::ServerStartExecution: {
        std::call_once(init_level, [&server_instance]() {
            auto &level = *server_instance.getMinecraft()->getLevel();
            auto &server = entt::locator<endstone::core::EndstoneServer>::value();
            hookEventHandler(*level.getActorEventCoordinator().actor_gameplay_handler);
            hookEventHandler(*level.getBlockEventCoordinator().block_gameplay_handler);
            hookEventHandler(*level.getItemEventCoordinator().item_gameplay_handler);
            hookEventHandler(*level.getLevelEventCoordinator().level_gameplay_handler);
            hookEventHandler(*level.getServerPlayerEventCoordinator().player_gameplay_handler);
            hookEventHandler(*level.getScriptingEventCoordinator().scripting_event_handler);
            hookEventHandler(*level.getServerNetworkEventCoordinator().server_network_event_handler);
            server.setLevel(level);
        });
        break;
    }
    default:
        break;
    }
}
