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

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/server.h"

namespace py = pybind11;

namespace endstone::detail {

void init_util(py::module_ &);
void init_command(py::module_ &);
void init_logger(py::module_ &);
void init_plugin(py::module_ &);
void init_permissions(py::module_ &, py::class_<Permissible> &permissible, py::class_<Permission> &permission,
                      py::enum_<PermissionDefault> &permission_default);
void init_server(py::class_<Server> &server);
void init_event(py::module_ &, py::class_<Event> &event, py::enum_<EventPriority> &event_priority);

PYBIND11_MODULE(endstone_python, m)  // NOLINT(*-use-anonymous-namespace)
{

    // Forward declaration, see:
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#avoiding-c-types-in-docstrings
    auto event = py::class_<Event>(m, "Event");
    auto event_priority = py::enum_<EventPriority>(
        m, "EventPriority",
        "Listeners are called in following order: LOWEST -> LOW -> NORMAL -> HIGH -> HIGHEST -> MONITOR");
    auto server = py::class_<Server>(m, "Server");
    auto permissible = py::class_<Permissible>(m, "Permissible");
    auto permission = py::class_<Permission>(m, "Permission");
    auto permission_default = py::enum_<PermissionDefault>(m, "PermissionDefault");

    init_util(m);
    init_command(m);
    init_logger(m);
    init_plugin(m);
    init_permissions(m, permissible, permission, permission_default);
    init_server(server);
    init_event(m, event, event_priority);
}

}  // namespace endstone::detail
