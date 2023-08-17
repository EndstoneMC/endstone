//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin_logger.h"
#include "endstone/plugin/python/python_plugin.h"
#include "endstone/plugin/python/python_plugin_description.h"

PYBIND11_MODULE(_plugin, m)
{
    py::class_<Plugin, PyPlugin>(m, "IPlugin") //
        .def(py::init<>())                     //
        .def("get_description", &Plugin::getDescription, py::return_value_policy::reference_internal)
        .def("on_load", &Plugin::onLoad)       //
        .def("on_enable", &Plugin::onEnable)   //
        .def("on_disable", &Plugin::onDisable) //
        .def("is_enabled", &Plugin::isEnabled) //
        .def("get_logger", &Plugin::getLogger, py::return_value_policy::reference_internal);

    py::class_<PluginDescription, PyPluginDescription>(m, "IPluginDescription") //
        .def(py::init<>())                                                      //
        .def("get_name", &PluginDescription::getName)                           //
        .def("get_version", &PluginDescription::getVersion)                     //
        .def("get_description", &PluginDescription::getDescription)             //
        .def("get_authors", &PluginDescription::getAuthors)                     //
        .def("get_prefix", &PluginDescription::getPrefix)                       //
        .def("get_fullname", &PluginDescription::getFullName);
}
