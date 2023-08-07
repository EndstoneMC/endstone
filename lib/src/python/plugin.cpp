//
// Created by Vincent on 21/07/2023.
//

#include "python/plugin.h"
#include "endstone.h"

void PyPlugin::onLoad()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_NAME(void,      // Return type
                           Plugin,    // Parent class
                           "on_load", // Name of function in python
                           onLoad,    // Name of function in C++
    );
}

void PyPlugin::onEnable()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_NAME(void,        // Return type
                           Plugin,      // Parent class
                           "on_enable", // Name of function in python
                           onEnable,    // Name of function in C++
    );
}

void PyPlugin::onDisable()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_NAME(void,         // Return type
                           Plugin,       // Parent class
                           "on_disable", // Name of function in python
                           onDisable,    // Name of function in C++
    );
}

PYBIND11_MODULE(_plugin, m)
{
    py::class_<Plugin, PyPlugin>(m, "Plugin") //
        .def(py::init<>())                    //
        .def("on_load", &Plugin::onLoad)      //
        .def("on_enable", &Plugin::onEnable)  //
        .def("on_disable", &Plugin::onDisable);
}
