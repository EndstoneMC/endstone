// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

// required if we want to assert in release mode
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

#include "endstone_core/endstone_server.h"

void testLogger(EndstoneServer &server)
{
    auto &logger = server.getLogger();
    logger.debug("Hello World!");
    logger.info("Hello World! Endstone version: {} (Minecraft: {})", server.getVersion(), server.getMinecraftVersion());
    logger.warning("Hello World!");
    logger.error("Hello World!");
    logger.critical("Hello World!");
}

auto constexpr CppTestPluginName = "CppTestPlugin";

void testPluginLoading(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();
    assert(plugin_manager.getPlugins().empty());
    assert(plugin_manager.getPlugin(CppTestPluginName) == nullptr);

    server.loadPlugins();
    assert(plugin_manager.getPlugins().size() == 1);
    assert(plugin_manager.getPlugin(CppTestPluginName) != nullptr);
}

void testPluginEnabling(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    auto *cpp_plugin = plugin_manager.getPlugin(CppTestPluginName);
    assert(cpp_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);

    server.enablePlugins();
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == true);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == true);
}

void testPluginDisabling(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    server.disablePlugins();

    auto *cpp_plugin = plugin_manager.getPlugin(CppTestPluginName);
    assert(cpp_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);
}

int main()
{
    auto &server = EndstoneServer::getInstance();
    testLogger(server);
    testPluginLoading(server);
    testPluginEnabling(server);
    testPluginDisabling(server);
    server.getLogger().info("All tests passed. Bye!");
}
