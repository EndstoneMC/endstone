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

#include "endstone/command/plugin_command.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"
#include "pybind/pybind.h"

class PyPlugin : public Plugin, public py::trampoline_self_life_support {
public:
    using Plugin::Plugin;

    [[nodiscard]] const PluginDescription &getDescription() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(const PluginDescription &, Plugin, "_get_description", getDescription);
    }

    void onLoad() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_load", onLoad);
    }

    void onEnable() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_enable", onEnable);
    }

    void onDisable() override
    {
        PYBIND11_OVERRIDE_NAME(void, Plugin, "on_disable", onDisable);
    }

    bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                   const std::vector<std::string> &args) noexcept override
    {
        PYBIND11_OVERRIDE_NAME(bool, Plugin, "on_command", onCommand, std::ref(sender), std::ref(command),
                               std::ref(label), std::ref(args));
    }
};

class PyPluginDescription : public PluginDescription {
public:
    using PluginDescription::PluginDescription;

    [[nodiscard]] std::optional<std::string> getDescription() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::string>, PluginDescription, "_get_description", getDescription);
    }

    [[nodiscard]] std::optional<std::vector<std::string>> getAuthors() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::vector<std::string>>, PluginDescription, "_get_authors", getAuthors);
    }

    [[nodiscard]] std::optional<std::string> getPrefix() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::string>, PluginDescription, "_get_prefix", getPrefix);
    }

    [[nodiscard]] std::vector<std::shared_ptr<Command>> getCommands() const override
    {
        PYBIND11_OVERRIDE_NAME(std::vector<std::shared_ptr<Command>>, PluginDescription, "_get_commands", getCommands);
    }
};

class PyPluginLoader : public PluginLoader, public py::trampoline_self_life_support {
public:
    using PluginLoader::PluginLoader;

    std::unique_ptr<Plugin> loadPlugin(const std::string &file) override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::unique_ptr<Plugin>, PluginLoader, "load_plugin", loadPlugin, std::ref(file));
    }

    [[nodiscard]] std::vector<std::string> getPluginFileFilters() const noexcept override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::vector<std::string>, PluginLoader, "_get_plugin_file_filters",
                                    getPluginFileFilters);
    }
};

void def_plugin(py::module &m)
{
    py::class_<Plugin, PyPlugin, py::smart_holder>(m, "PluginBase")
        .def(py::init<>())
        .def("_get_description", &Plugin::getDescription, py::return_value_policy::reference)
        .def("on_load", &Plugin::onLoad)
        .def("on_enable", &Plugin::onEnable)
        .def("on_disable", &Plugin::onDisable)
        .def("on_command", &Plugin::onCommand, py::arg("sender"), py::arg("command"), py::arg("label"), py::arg("args"))
        .def("get_command", &Plugin::getCommand, py::arg("name"), py::return_value_policy::reference)
        .def("_get_logger", &Plugin::getLogger, py::return_value_policy::reference)
        .def("_get_plugin_loader", &Plugin::getPluginLoader, py::return_value_policy::reference)
        .def("_get_server", &Plugin::getServer, py::return_value_policy::reference)
        .def_property_readonly("enabled", &Plugin::isEnabled);

    py::class_<PluginDescription, PyPluginDescription>(m, "PluginDescription")
        .def(py::init<const std::string &, const std::string &>(), py::arg("name"), py::arg("version"))
        .def_property_readonly("name", &PluginDescription::getName, py::return_value_policy::reference)
        .def_property_readonly("version", &PluginDescription::getVersion, py::return_value_policy::reference)
        .def_property_readonly("full_name", &PluginDescription::getFullName, py::return_value_policy::reference)
        .def("_get_description", &PluginDescription::getDescription)
        .def("_get_authors", &PluginDescription::getAuthors)
        .def("_get_prefix", &PluginDescription::getPrefix)
        .def("_get_commands", &PluginDescription::getCommands);

    py::class_<PluginLoader, PyPluginLoader, py::smart_holder>(m, "PluginLoader")
        .def(py::init<Server &>(), py::arg("server"))
        .def("load_plugin", &PluginLoader::loadPlugin, py::arg("file"))
        .def("_get_plugin_file_filters", &PluginLoader::getPluginFileFilters);
}
