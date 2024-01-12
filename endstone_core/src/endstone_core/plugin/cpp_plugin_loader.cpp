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

#include "endstone_core/plugin/cpp_plugin_loader.h"

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <dlfcn.h>
#endif

#include "endstone/plugin/plugin.h"
#include "endstone_core/endstone_server.h"
#include "endstone_core/logger_factory.h"

#ifdef _WIN32

std::shared_ptr<Plugin> CppPluginLoader::loadPlugin(const std::string &file) noexcept
{
    auto &logger = getServer().getLogger();

    auto *module = LoadLibraryA(file.c_str());
    if (!module) {
        logger.error("Failed to load c++ plugin from {}: LoadLibrary failed with code {}.", file, GetLastError());
        return nullptr;
    }

    using PluginInit = Plugin *(*)();
    auto plugin_init = GetProcAddress(module, "EndstonePluginInit");
    if (!plugin_init) {
        FreeLibrary(module);
        logger.error("Failed to load c++ plugin from {}: No entry point. Did you forget ENDSTONE_PLUGIN?", file);
        return nullptr;
    }

    auto *plugin = reinterpret_cast<PluginInit>(plugin_init)();
    if (!plugin) {
        FreeLibrary(module);
        logger.error("Failed to load c++ plugin from {}: Invalid plugin instance.", file);
        return nullptr;
    }

    initPlugin(*plugin, LoggerFactory::getLogger(plugin->getDescription().getName()));
    return std::shared_ptr<Plugin>(plugin);
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const
{
    return {"\\.dll$"};
}

#elif __linux__

std::shared_ptr<Plugin> CppPluginLoader::loadPlugin(const std::string &file) noexcept
{
    auto &logger = getServer().getLogger();

    auto *module = dlopen(file.c_str(), RTLD_NOW);
    if (!module) {
        logger.error("Failed to load c++ plugin from {}: dlopen failed with code {}.", file, dlerror());
        return nullptr;
    }

    using PluginInit = Plugin *(*)();
    auto plugin_init = dlsym(module, "EndstonePluginInit");
    if (!plugin_init) {
        dlclose(module);
        logger.error("Failed to load c++ plugin from {}: No entry point. Did you forget ENDSTONE_PLUGIN?", file);
        return nullptr;
    }

    auto *plugin = reinterpret_cast<PluginInit>(plugin_init)();
    if (!plugin) {
        dlclose(module);
        logger.error("Failed to load c++ plugin from {}: Invalid plugin instance.", file);
        return nullptr;
    }

    initPlugin(*plugin, LoggerFactory::getLogger(plugin->getDescription().getName()));
    return std::shared_ptr<Plugin>(plugin);
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const
{
    return {"\\.so$"};
}

#endif
