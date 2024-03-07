// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <pybind11/pybind11.h>

#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::detail {

void init_command(py::module_ &);
void init_logger(py::module_ &);
void init_plugin(py::module_ &);
void init_server(py::module_ &, py::class_<Server> &server);
void init_util(py::module_ &);

PYBIND11_MODULE(endstone_python, m)  // NOLINT(*-use-anonymous-namespace)
{

    // Forward declaration, see:
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#avoiding-c-types-in-docstrings
    auto server = py::class_<Server>(m, "Server");
    init_command(m);
    init_logger(m);
    init_plugin(m);
    init_server(m, server);
    init_util(m);
}

}  // namespace endstone::detail
