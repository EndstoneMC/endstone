//
// Created by Vincent on 21/07/2023.
//

#include "plugin.h"
#include "enstone.h"

void PythonPlugin::on_enable()
{
    /* Trampoline (need one for each virtual function) */
    PYBIND11_OVERLOAD_PURE(void,       /* Return type */
                           BasePlugin, /* Parent class */
                           on_enable,  /* Name of function in C++ (must match Python name) */
    );
}

void PythonPlugin::on_disable()
{
    /* Trampoline (need one for each virtual function) */
    PYBIND11_OVERLOAD_PURE(void,       /* Return type */
                           BasePlugin, /* Parent class */
                           on_disable, /* Name of function in C++ (must match Python name) */
    );
}

PYBIND11_MODULE(plugin, m)
{
    py::class_<BasePlugin>(m, "BasePlugin")       //
        .def("on_enable", &BasePlugin::on_enable) //
        .def("on_disable", &BasePlugin::on_disable);

    py::class_<PythonPlugin, BasePlugin>(m, "PythonPlugin") //
        .def(py::init<>())                                  //
        .def("on_enable", &BasePlugin::on_enable)           //
        .def("on_disable", &BasePlugin::on_disable);
}
