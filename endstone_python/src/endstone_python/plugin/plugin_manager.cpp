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

#include "endstone/plugin/plugin_manager.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/logger.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"
#include "endstone_python/endstone_python.h"

namespace py = pybind11;

void def_plugin_manager(py::module &m)
{
    py::class_<PluginManager>(m, "PluginManager")
        .def("get_plugin", &PluginManager::getPlugin, py::arg("name"), py::return_value_policy::reference)
        .def_property_readonly("plugins", &PluginManager::getPlugins)
        .def("is_plugin_enabled", py::overload_cast<const std::string &>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"))
        .def("is_plugin_enabled", py::overload_cast<Plugin *>(&PluginManager::isPluginEnabled, py::const_),
             py::arg("plugin"))
        .def("load_plugins", &PluginManager::loadPlugins, py::arg("directory"))
        .def("enable_plugin", &PluginManager::enablePlugin, py::arg("plugin"))
        .def("disable_plugin", &PluginManager::disablePlugin, py::arg("plugin"))
        .def("disable_plugins", &PluginManager::disablePlugins)
        .def("clear_plugins", &PluginManager::clearPlugins);
}
