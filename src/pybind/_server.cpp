//
// Created by Vincent on 17/08/2023.
//

#include "endstone/server.h"
#include "pybind.h"

void def_server(py::module &m)
{
    py::class_<Server>(m, "Server");
}
