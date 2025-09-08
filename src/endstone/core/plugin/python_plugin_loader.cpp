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

#include "endstone/core/plugin/python_plugin_loader.h"

#include <pybind11/embed.h>
namespace py = pybind11;

#include "endstone/core/logger_factory.h"

namespace endstone::core {

PythonPluginLoader::PythonPluginLoader(Server &server) : PluginLoader(server)
{
    try {
        py::gil_scoped_acquire gil{};
        auto module = py::module_::import("endstone.plugin.plugin_loader");
        auto cls = module.attr("PythonPluginLoader");
        obj_ = cls(std::ref(server));
    }
    catch (std::exception &e) {
        server.getLogger().error("Error occurred when trying to register a plugin loader: {}", e.what());
        throw;
    }
}

PythonPluginLoader::~PythonPluginLoader()
{
    py::gil_scoped_acquire gil{};
    (void)obj_.dec_ref();
    obj_.release();
}

Plugin *PythonPluginLoader::loadPlugin(std::string file)
{
    return pimpl()->loadPlugin(file);
}

std::vector<Plugin *> PythonPluginLoader::loadPlugins(std::string directory)
{
    return pimpl()->loadPlugins(directory);
}

std::vector<std::string> PythonPluginLoader::getPluginFileFilters() const
{
    return pimpl()->getPluginFileFilters();
}

void PythonPluginLoader::enablePlugin(Plugin &plugin) const
{
    pimpl()->enablePlugin(plugin);
}

void PythonPluginLoader::disablePlugin(Plugin &plugin) const
{
    pimpl()->disablePlugin(plugin);
}

PluginLoader *PythonPluginLoader::pimpl() const
{
    return obj_.cast<PluginLoader *>();
}

}  // namespace endstone::core
