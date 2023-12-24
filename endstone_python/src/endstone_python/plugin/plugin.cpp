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

#include "endstone/plugin/plugin.h"

#include <pybind11/pybind11.h>

#include "endstone/logger.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"
#include "endstone_python/endstone_python.h"

namespace py = pybind11;

class PyPlugin : public Plugin {
public:
    using Plugin::Plugin;

    [[nodiscard]] const PluginDescription &getDescription() const noexcept override
    {
        try {
            PYBIND11_OVERRIDE_PURE_NAME(const PluginDescription &, Plugin, "_get_description", getDescription);
        }
        catch (std::exception &e) {
            getLogger().error(e.what());
            std::terminate();
        }
    }

    void onLoad() noexcept override
    {
        try {
            PYBIND11_OVERRIDE_NAME(void, Plugin, "on_load", onLoad);
        }
        catch (std::exception &e) {
            getLogger().error("Error occurred when loading {}", getDescription().getFullName());
            getLogger().error(e.what());
        }
    }

    void onEnable() noexcept override
    {
        try {
            PYBIND11_OVERRIDE_NAME(void, Plugin, "on_enable", onEnable);
        }
        catch (std::exception &e) {
            getLogger().error("Error occurred when enabling {}.", getDescription().getFullName());
            getLogger().error(e.what());
        }
    }

    void onDisable() noexcept override
    {
        try {
            PYBIND11_OVERRIDE_NAME(void, Plugin, "on_disable", onDisable);
        }
        catch (std::exception &e) {
            getLogger().error("Error occurred when disabling {}.", getDescription().getFullName());
            getLogger().error(e.what());
        }
    }
};

void def_plugin(py::module &m)
{
    py::class_<Plugin, PyPlugin>(m, "Plugin")
        .def(py::init<>())
        .def("on_load", &Plugin::onLoad)
        .def("on_enable", &Plugin::onEnable)
        .def("on_disable", &Plugin::onDisable)
        .def("_get_description", &Plugin::getDescription, py::return_value_policy::reference)
        .def_property_readonly("logger", &Plugin::getLogger, py::return_value_policy::reference)
        .def_property_readonly("plugin_loader", &Plugin::getPluginLoader, py::return_value_policy::reference)
        .def_property_readonly("server", &Plugin::getServer, py::return_value_policy::reference)
        .def_property_readonly("enabled", &Plugin::isEnabled);
}
