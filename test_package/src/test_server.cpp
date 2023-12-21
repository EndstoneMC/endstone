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

int main()
{
    auto &server = EndstoneServer::getInstance();
    auto &logger = server.getLogger();

    logger.debug("Hello World!");
    logger.info("Hello World!");
    logger.warning("Hello World!");
    logger.error("Hello World!");
    logger.critical("Hello World!");

    logger.info("Endstone version: v{} (Minecraft: v{})", server.getVersion(), server.getMinecraftVersion());

    auto constexpr CppTestPluginName = "CppTestPlugin";

    auto &plugin_manager = server.getPluginManager();
    assert(plugin_manager.getPlugins().size() == 0);
    assert(plugin_manager.getPlugin(CppTestPluginName) == nullptr);

    server.loadPlugins();
    assert(plugin_manager.getPlugins().size() == 1);

    auto *plugin = plugin_manager.getPlugin(CppTestPluginName);
    assert(plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);

    server.enablePlugins();
    assert(plugin_manager.isPluginEnabled(plugin) == true);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == true);

    server.disablePlugins();
    assert(plugin_manager.isPluginEnabled(plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);

    server.getLogger().info("Bye!");
}
