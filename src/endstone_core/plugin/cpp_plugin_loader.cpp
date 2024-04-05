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

#include "endstone/detail/plugin/cpp_plugin_loader.h"

#ifdef _WIN32
#include <Windows.h>
#define LOAD_LIBRARY(file)             LoadLibraryA(file)
#define GET_FUNCTION(module, function) GetProcAddress(module, function)
#define GET_ERROR()                    GetLastError()
#define CLOSE_LIBRARY(module)          FreeLibrary(module)
#elif __linux__
#include <dlfcn.h>
#define LOAD_LIBRARY(file)             dlopen(file, RTLD_NOW)
#define GET_FUNCTION(module, function) dlsym(module, function)
#define GET_ERROR()                    dlerror()
#define CLOSE_LIBRARY(module)          dlclose(module)
#endif

#include <filesystem>
#include <regex>
namespace fs = std::filesystem;

#include "endstone/detail/logger_factory.h"
#include "endstone/detail/server.h"
#include "endstone/plugin/plugin.h"

namespace endstone::detail {

std::vector<Plugin *> CppPluginLoader::loadPlugins(const std::string &directory)
{
    auto &logger = server_.getLogger();

    auto dir = fs::path(directory);
    if (!exists(dir)) {
        logger.error("Error occurred when trying to load plugins in '{}': Provided directory does not exist.",
                     dir.string());
        return {};
    }

    if (!is_directory(dir)) {
        logger.error("Error occurred when trying to load plugins in '{}': Provided path is not a directory.",
                     dir.string());
        return {};
    }

    std::vector<Plugin *> loaded_plugins;

    for (const auto &entry : fs::directory_iterator(dir)) {
        fs::path file;

        if (!is_regular_file(entry.status())) {
            continue;
        }

        file = entry.path();

        for (const auto &pattern : getPluginFileFilters()) {
            std::regex r(pattern);
            if (std::regex_search(file.string(), r)) {
                auto plugin = loadPlugin(file.string());
                if (plugin) {
                    loaded_plugins.push_back(plugin.get());
                    plugins_.push_back(std::move(plugin));
                }
            }
        }
    }

    return loaded_plugins;
}

std::unique_ptr<Plugin> CppPluginLoader::loadPlugin(const std::string &file)
{
    auto &logger = server_.getLogger();
    auto path = fs::path(file);
    if (!exists(path)) {
        logger.error("Could not load plugin from '{}': Provided file does not exist.", path.string());
        return nullptr;
    }

    auto *module = LOAD_LIBRARY(file.c_str());
    if (!module) {
        logger.error("Failed to load c++ plugin from {}: LoadLibrary failed with code {}.", file, GET_ERROR());
        return nullptr;
    }

    using InitPlugin = Plugin *(*)();
    auto init_plugin = GET_FUNCTION(module, "init_endstone_plugin");
    if (!init_plugin) {
        CLOSE_LIBRARY(module);
        logger.error("Failed to load c++ plugin from {}: No entry point. Did you forget ENDSTONE_PLUGIN?", file);
        return nullptr;
    }

    auto *plugin = reinterpret_cast<InitPlugin>(init_plugin)();
    if (!plugin) {
        CLOSE_LIBRARY(module);
        logger.error("Failed to load c++ plugin from {}: Invalid plugin instance.", file);
        return nullptr;
    }

    static const std::string supported_api_version = ENDSTONE_API_VERSION;
    if (plugin->getDescription().getAPIVersion() != supported_api_version) {
        logger.error("Error occurred when trying to load plugin '{}': plugin was compiled for Endstone "
                     "API version: {}, but the server has an incompatible API version: {}.",
                     plugin->getDescription().getName(), plugin->getDescription().getAPIVersion(),
                     supported_api_version);
        return nullptr;
    }

    auto logger_name = plugin->getDescription().getPrefix();
    if (logger_name.empty()) {
        logger_name = plugin->getDescription().getName();
    }
    initPlugin(*plugin, LoggerFactory::getLogger(logger_name));
    return std::unique_ptr<Plugin>(plugin);
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const
{
    return {_WIN32_LINUX_("\\.dll$", "\\.so$")};
}

}  // namespace endstone::detail

#undef LOAD_LIBRARY
#undef GET_FUNCTION
#undef GET_ERROR
#undef CLOSE_LIBRARY
