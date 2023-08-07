//
// Created by Vincent on 22/07/2023.
//

#include "python/plugin_manager.h"
#include "python/server.h"

Plugin *PyPluginManager::loadPlugin(const std::string &path)
{
    PYBIND11_OVERRIDE_PURE_NAME(Plugin *,      //
                                PluginManager, //
                                "load_plugin", //
                                loadPlugin     //
    );
}

std::vector<Plugin *> PyPluginManager::loadPlugins(const std::string &directory)
{
    PYBIND11_OVERRIDE_PURE_NAME(std::vector<Plugin *>, //
                                PluginManager,         //
                                "load_plugins",        //
                                loadPlugins            //
    );
}

PYBIND11_MODULE(_plugin_manager, m)
{
    py::class_<PluginManager, PyPluginManager>(m, "PluginManager")
        .def(py::init<Server *>(), py::arg("server"))
        .def("load_plugin", &PluginManager::loadPlugin, py::arg("path"))
        .def("load_plugins", &PluginManager::loadPlugins, py::arg("directory"));
}
