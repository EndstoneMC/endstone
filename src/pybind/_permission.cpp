//
// Created by Vincent on 29/08/2023.
//

#include "endstone/common.h"
#include "endstone/permission/permissible.h"
#include "pybind.h"

PYBIND11_MODULE(_permission, m)
{
    py::class_<Permissible>(m, "Permissible");
}
