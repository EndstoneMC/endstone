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

#include <pybind11/embed.h>
namespace py = pybind11;

#include "endstone_core/endstone_server.h"

void testLogger(EndstoneServer &server)
{
    auto &logger = server.getLogger();
    logger.debug("Hello World!");
    logger.info("Hello World! Endstone version: v{} (Minecraft: v{})", server.getVersion(),
                server.getMinecraftVersion());
    logger.warning("Hello World!");
    logger.error("Hello World!");
    logger.critical("Hello World!");
}

void testPythonInterpreter(EndstoneServer & /*server*/)
{
    py::gil_scoped_acquire gil{};
    py::print("Hello World from Python!");
}

auto constexpr CppTestPluginName = "CppTestPlugin";
auto constexpr PythonTestPluginName = "PythonTestPlugin";

void testPluginLoading(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();
    assert(plugin_manager.getPlugins().empty());
    assert(plugin_manager.getPlugin(CppTestPluginName) == nullptr);
    assert(plugin_manager.getPlugin(PythonTestPluginName) == nullptr);

    server.loadPlugins();
    assert(plugin_manager.getPlugins().size() == 2);
    assert(plugin_manager.getPlugin(CppTestPluginName) != nullptr);
    assert(plugin_manager.getPlugin(PythonTestPluginName) != nullptr);
}

void testPluginEnabling(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    auto *cpp_plugin = plugin_manager.getPlugin(CppTestPluginName);
    assert(cpp_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);

    auto *python_plugin = plugin_manager.getPlugin(PythonTestPluginName);
    assert(python_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(python_plugin) == false);
    assert(plugin_manager.isPluginEnabled(PythonTestPluginName) == false);

    server.enablePlugins();
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == true);
    assert(plugin_manager.isPluginEnabled(python_plugin) == true);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == true);
    assert(plugin_manager.isPluginEnabled(PythonTestPluginName) == true);
}

void testPluginDisabling(EndstoneServer &server)
{
    auto &plugin_manager = server.getPluginManager();

    server.disablePlugins();

    auto *cpp_plugin = plugin_manager.getPlugin(CppTestPluginName);
    assert(cpp_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(cpp_plugin) == false);
    assert(plugin_manager.isPluginEnabled(CppTestPluginName) == false);

    auto *python_plugin = plugin_manager.getPlugin(PythonTestPluginName);
    assert(python_plugin != nullptr);
    assert(plugin_manager.isPluginEnabled(python_plugin) == false);
    assert(plugin_manager.isPluginEnabled(PythonTestPluginName) == false);
}

int main()
{
    auto &server = EndstoneServer::getInstance();
    testLogger(server);
    testPythonInterpreter(server);
    testPluginLoading(server);
    testPluginEnabling(server);
    testPluginDisabling(server);
    server.getLogger().info("All tests passed. Bye!");
}
