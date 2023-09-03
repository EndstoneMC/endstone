//
// Created by Vincent on 17/08/2023.
//

#include "endstone/command/plugin_command.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"
#include "pybind.h"

class PyPlugin : public Plugin, public py::trampoline_self_life_support {
public:
    using Plugin::Plugin;

    const PluginDescription &getDescription() const override
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

    std::optional<std::string> getDescription() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::string>, PluginDescription, "_get_description", getDescription);
    }

    std::optional<std::vector<std::string>> getAuthors() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::vector<std::string>>, PluginDescription, "_get_authors", getAuthors);
    }

    std::optional<std::string> getPrefix() const override
    {
        PYBIND11_OVERRIDE_NAME(std::optional<std::string>, PluginDescription, "_get_prefix", getPrefix);
    }

    std::vector<std::shared_ptr<Command>> getCommands() const override
    {
        PYBIND11_OVERRIDE_NAME(std::vector<std::shared_ptr<Command>>, PluginDescription, "_get_commands", getCommands);
    }
};

class PyPluginLoader : public PluginLoader {
public:
    using PluginLoader::PluginLoader;

    std::unique_ptr<Plugin> loadPlugin(const std::string &file) override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::unique_ptr<Plugin>, PluginLoader, "load_plugin", loadPlugin, std::ref(file));
    }

    std::vector<std::string> getPluginFileFilters() const noexcept override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::vector<std::string>, PluginLoader, "_get_plugin_file_filters",
                                    getPluginFileFilters);
    }
};

void def_plugin(py::module &m)
{
    py::class_<Plugin, PyPlugin, py::smart_holder>(m, "PluginBase")
        .def(py::init<>())
        .def("_get_description", &Plugin::getDescription)
        .def("on_load", &Plugin::onLoad)
        .def("on_enable", &Plugin::onEnable)
        .def("on_disable", &Plugin::onDisable)
        .def("on_command", &Plugin::onCommand, py::arg("sender"), py::arg("command"), py::arg("label"), py::arg("args"))
        .def("get_command", &Plugin::getCommand, py::arg("name"))
        .def("_get_logger", &Plugin::getLogger)
        .def("_get_plugin_loader", &Plugin::getPluginLoader)
        .def("_get_server", &Plugin::getServer)
        .def_property_readonly("enabled", &Plugin::isEnabled);

    py::class_<PluginDescription, PyPluginDescription>(m, "PluginDescription")
        .def(py::init<const std::string &, const std::string &>(), py::arg("name"), py::arg("version"))
        .def_property_readonly("name", &PluginDescription::getName)
        .def_property_readonly("version", &PluginDescription::getVersion)
        .def_property_readonly("full_name", &PluginDescription::getFullName)
        .def("_get_description", &PluginDescription::getDescription)
        .def("_get_authors", &PluginDescription::getAuthors)
        .def("_get_prefix", &PluginDescription::getPrefix)
        .def("_get_commands", &PluginDescription::getCommands);

    py::class_<PluginLoader, PyPluginLoader, py::smart_holder>(m, "PluginLoader")
        .def(py::init<Server &>(), py::arg("server"))
        .def("load_plugin", &PluginLoader::loadPlugin, py::arg("file"))
        .def("_get_plugin_file_filters", &PluginLoader::getPluginFileFilters);
}
