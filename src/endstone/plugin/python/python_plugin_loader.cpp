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

#include "endstone/plugin/python/python_plugin_loader.h"

#include <memory>
#include <string>
#include <vector>

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_logger.h"
#include "endstone/server.h"
#include "pybind/pybind.h"

PythonPluginLoader::PythonPluginLoader(Server &server, const std::string &module_name,
                                       const std::string &class_name) noexcept
    : PluginLoader(server)
{
    py::gil_scoped_acquire gil{};
    auto module = py::module_::import(module_name.c_str());
    auto cls = module.attr(class_name.c_str());
    auto py_loader = cls(std::ref(server));
    loader_ = py_loader.cast<std::unique_ptr<PluginLoader>>();
}

std::unique_ptr<Plugin> PythonPluginLoader::loadPlugin(const std::string &file) noexcept
{
    auto plugin = loader_->loadPlugin(file);
    if (plugin) {
        initPlugin(*plugin, std::make_unique<PluginLogger>(*plugin));
        return plugin;
    }

    return nullptr;
}

std::vector<std::string> PythonPluginLoader::getPluginFileFilters() const noexcept
{
    return std::move(loader_->getPluginFileFilters());
}

void PythonPluginLoader::enablePlugin(Plugin &plugin) const noexcept
{
    loader_->enablePlugin(plugin);
}

void PythonPluginLoader::disablePlugin(Plugin &plugin) const noexcept
{
    loader_->disablePlugin(plugin);
}
