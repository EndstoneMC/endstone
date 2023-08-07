//
// Created by Vincent on 31/07/2023.
//

#include "python/server.h"

Server::Server()
{
    printf("Creating EndStone Server...\n");
    try
    {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone.plugin");
        printf("Python module imported\n");
        auto cls = module.attr("PluginManager");
        printf("Python class found\n");
        pluginManager_ = cls(*this);
        printf("Instantiation done\n");
    }
    catch (py::error_already_set &e)
    {
        printf("Python error: %s", e.what());
    }
}

PYBIND11_MODULE(_server, m)
{
    py::class_<Server>(m, "Server") //
        .def_static("get_instance", &Server::getInstance);
}
