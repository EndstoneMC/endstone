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

#ifdef _WIN32

#include <Windows.h>

#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_logger.h"

std::unique_ptr<Plugin> CppPluginLoader::loadPlugin(const std::string &file)
{
    HMODULE module = LoadLibraryA(file.c_str());
    if (!module) {
        throw std::system_error(GetLastError(), std::system_category(), "LoadLibrary failed");
    }

    using fp = Plugin *(*)();
    auto func = GetProcAddress(module, "createPlugin");

    if (!func) {
        FreeLibrary(module);
        throw std::runtime_error("Failed to find createPlugin function in DLL: " + file +
                                 ". Did you forget ENDSTONE_PLUGIN_CLASS?");
    }

    auto createPlugin = reinterpret_cast<fp>(func);
    auto plugin = createPlugin();

    if (!plugin) {
        FreeLibrary(module);  // First, free the loaded library to clean up resources.
        throw std::runtime_error("Failed to create a plugin instance from DLL: " + file + ". Invalid plugin instance.");
    }

    initPlugin(*plugin, std::make_shared<PluginLogger>(*plugin));
    return std::unique_ptr<Plugin>(plugin);
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const noexcept
{
    return {"\\.dll$"};
}

#endif  //_WIN32
