//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin_description.h"
#include "pybind.h"

/*
 * All the plugin-related python trampoline classes are removed since the following commit
 * https://github.com/EndstoneMC/endstone/commit/c579df43332abf9c7f73553648ca47e34256fec7
 * Record here in case we need them back in the future...
 */

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
        PYBIND11_OVERRIDE_NAME(std::optional<std::string>, PluginDescription, "_get_prefix", getDescription);
    }

    std::vector<std::shared_ptr<Command>> getCommands() const override
    {
        PYBIND11_OVERRIDE_NAME(std::vector<std::shared_ptr<Command>>, PluginDescription, "_get_commands", getCommands);
    }
};

PYBIND11_MODULE(_plugin, m)
{
    py::class_<PluginDescription, PyPluginDescription, std::shared_ptr<PluginDescription>>(m, "PluginDescription")
        .def(py::init<const std::string &, const std::string &>(), py::arg("name"), py::arg("version"))
        .def_property_readonly("name", &PluginDescription::getName)
        .def_property_readonly("version", &PluginDescription::getVersion)
        .def_property_readonly("full_name", &PluginDescription::getFullName)
        .def("_get_description", &PluginDescription::getDescription)
        .def("_get_authors", &PluginDescription::getAuthors)
        .def("_get_prefix", &PluginDescription::getPrefix)
        .def("_get_commands", &PluginDescription::getCommands);
}
