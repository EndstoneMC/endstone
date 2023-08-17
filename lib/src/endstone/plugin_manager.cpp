//
// Created by Vincent on 22/07/2023.
//

#include "endstone/plugin_manager.h"
#include "endstone/server.h"

Plugin *PyPluginManager::loadPlugin(const std::string &path)
{
    PYBIND11_OVERRIDE_PURE_NAME(Plugin *,      // Return value
                                PluginManager, // Parent class
                                "load_plugin", // Name of function in Python
                                loadPlugin     // Name of function in C++
    );
}

std::vector<Plugin *> PyPluginManager::loadPlugins(const std::string &directory)
{
    PYBIND11_OVERRIDE_PURE_NAME(std::vector<Plugin *>, // Return value
                                PluginManager,         // Parent class
                                "load_plugins",        // Name of function in Python
                                loadPlugins            // Name of function in C++
    );
}

void PyPluginManager::enablePlugin(const Plugin *plugin) const
{
    PYBIND11_OVERRIDE_PURE_NAME(void,            // Return value
                                PluginManager,   // Parent class
                                "enable_plugin", // Name of function in Python
                                enablePlugin     // Name of function in C++
    );
}

void PyPluginManager::enablePlugins() const
{
    PYBIND11_OVERRIDE_PURE_NAME(void,             // Return value
                                PluginManager,    // Parent class
                                "enable_plugins", // Name of function in Python
                                enablePlugins     // Name of function in C++
    );
}

void PyPluginManager::disablePlugin(const Plugin *plugin) const
{
    PYBIND11_OVERRIDE_PURE_NAME(void,             // Return value
                                PluginManager,    // Parent class
                                "disable_plugin", // Name of function in Python
                                disablePlugin     // Name of function in C++
    );
}

void PyPluginManager::disablePlugins() const
{
    PYBIND11_OVERRIDE_PURE_NAME(void,              // Return value
                                PluginManager,     // Parent class
                                "disable_plugins", // Name of function in Python
                                disablePlugins     // Name of function in C++
    );
}

void PyPluginManager::clearPlugins()
{
    PYBIND11_OVERRIDE_PURE_NAME(void,            // Return value
                                PluginManager,   // Parent class
                                "clear_plugins", // Name of function in Python
                                clearPlugins     // Name of function in C++
    );
}

PYBIND11_MODULE(_plugin_manager, m)
{
    py::class_<PluginManager, PyPluginManager>(m, "PluginManager")
        .def(py::init<Server *>(), py::arg("server"))
        .def("load_plugin", &PluginManager::loadPlugin, py::arg("path"))
        .def("load_plugins", &PluginManager::loadPlugins, py::arg("directory"))  //
        .def("enable_plugin", &PluginManager::enablePlugin, py::arg("plugin"))   //
        .def("enable_plugins", &PluginManager::enablePlugins)                    //
        .def("disable_plugin", &PluginManager::disablePlugin, py::arg("plugin")) //
        .def("disable_plugins", &PluginManager::disablePlugins)                  //
        .def("clear_plugins", &PluginManager::clearPlugins);
}
