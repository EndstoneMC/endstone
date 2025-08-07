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

#include "endstone/core/plugin/cpp_plugin_loader.h"

#include <filesystem>
#include <regex>
namespace fs = std::filesystem;

#include "endstone/core/logger_factory.h"
#include "endstone/core/server.h"
#include "endstone/plugin/plugin.h"

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

namespace endstone::core {

CppPluginLoader::CppPluginLoader(Server &server) : PluginLoader(server)
{
    // prepare the temp folder
    prefix_ = fs::current_path() / "plugins" / ".local";
    if (!exists(prefix_)) {
        create_directory(prefix_);
    }

    // remove old plugin files
    for (const auto &entry : std::filesystem::directory_iterator(prefix_)) {
        if (!is_regular_file(entry.status())) {
            continue;
        }
        const auto &file = entry.path();
        for (const auto &pattern : CppPluginLoader::getPluginFileFilters()) {
            if (std::regex r(pattern); std::regex_search(file.string(), r)) {
                remove(file);
            }
        }
    }
}

Plugin *CppPluginLoader::loadPlugin(std::string file)
{
    auto &logger = server_.getLogger();
    auto path = absolute(fs::path(file));

    // Check if file exists
    if (!exists(path)) {
        logger.error("Could not load plugin from '{}': Provided file does not exist.", path);
        return nullptr;
    }

    // Make a shadow copy
    auto hash = std::hash<std::string>{}(path.string());
    fs::path temp_path = prefix_ / (fmt::format("{}-{:016x}", path.stem(), hash) + path.extension().string());
    try {
        copy_file(path, temp_path, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error &e) {
        logger.error("Could not load plugin from '{}': {}", path, e.what());
        return nullptr;
    }

    // Load from the copy to allow safe reloading and hot-swapping
    // https://github.com/EndstoneMC/endstone/issues/228
    auto *module = LOAD_LIBRARY(temp_path.string().c_str());
    if (!module) {
        logger.error("Failed to load c++ plugin from {}: LoadLibrary failed with code {}.", path, GET_ERROR());
        return nullptr;
    }

    using InitPlugin = Plugin *(*)();
    auto init_plugin = GET_FUNCTION(module, "init_endstone_plugin");
    if (!init_plugin) {
        CLOSE_LIBRARY(module);
        logger.error("Failed to load c++ plugin from {}: No entry point. Did you forget ENDSTONE_PLUGIN?", path);
        return nullptr;
    }

    auto *plugin = reinterpret_cast<InitPlugin>(init_plugin)();
    if (!plugin) {
        CLOSE_LIBRARY(module);
        logger.error("Failed to load c++ plugin from {}: Invalid plugin instance.", path);
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

    return plugins_
        .emplace_back(plugin,
                      [module](const Plugin *plugin) {
                          delete plugin;
                          CLOSE_LIBRARY(module);
                      })
        .get();
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const
{
#ifdef _WIN32
    return {"\\.dll$"};
#elif __linux__
    return {"\\.so$"};
#endif
}

}  // namespace endstone::core

#undef LOAD_LIBRARY
#undef GET_FUNCTION
#undef GET_ERROR
#undef CLOSE_LIBRARY
