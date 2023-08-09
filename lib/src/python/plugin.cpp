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
Logger &PyPlugin::getLogger()
{
    // Trampoline (need one for each virtual function)
    PYBIND11_OVERLOAD_PURE_NAME(Logger &,     // Return type
                                Plugin,       // Parent class
                                "get_logger", // Name of function in python
                                getLogger,    // Name of function in C++
    );
}

bool Plugin::isEnabled() const
{
    return enabled_;
}

void Plugin::setEnabled(bool enabled)
{
    if (enabled_ != enabled)
    {
        enabled_ = enabled;

        if (enabled_)
        {
            onEnable();
        }
        else
        {
            onDisable();
        }
    }
}

PYBIND11_MODULE(_plugin, m)
{
    py::class_<Plugin, PyPlugin>(m, "Plugin")  //
        .def(py::init<>())                     //
        .def("on_load", &Plugin::onLoad)       //
        .def("on_enable", &Plugin::onEnable)   //
        .def("on_disable", &Plugin::onDisable) //
        .def("is_enabled", &Plugin::isEnabled) //
        .def("_set_enabled", &Plugin::setEnabled, py::arg("enabled"))
        .def("get_logger", &Plugin::getLogger, py::return_value_policy::reference_internal);
}
