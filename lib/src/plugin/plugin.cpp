//
// Created by Vincent on 21/07/2023.
//

#include "plugin.h"
#include "endstone.h"
#include "endstone_py.h"

void PythonPlugin::onEnable()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_NAME(void,        // Return type
                           BasePlugin,  // Parent class
                           "on_enable", // Name of function in python
                           onEnable,    // Name of function in C++
    );
}

void PythonPlugin::onDisable()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_NAME(void,         // Return type
                           BasePlugin,   // Parent class
                           "on_disable", // Name of function in python
                           onDisable,    // Name of function in C++
    );
}

PYBIND11_MODULE(plugin, m)
{
    py::class_<BasePlugin>(m, "BasePlugin")      //
        .def("on_enable", &BasePlugin::onEnable) //
        .def("on_disable", &BasePlugin::onDisable);

    py::class_<PythonPlugin, BasePlugin>(m, "PythonPlugin") //
        .def(py::init<>())                                  //
        .def("on_enable", &BasePlugin::onEnable)            //
        .def("on_disable", &BasePlugin::onDisable);
}
