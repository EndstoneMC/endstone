//
// Created by Vincent on 17/08/2023.
//

#include "endstone/server.h"
#include "pybind.h"

PYBIND11_MODULE(_server, m)
{
    py::class_<Server>(m, "Server");
}
