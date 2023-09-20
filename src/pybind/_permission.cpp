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

#include <utility>

#include "endstone/permission/permission.h"
#include "pybind/pybind.h"

void def_permission(py::module &m)
{
    py::class_<Permission, py::smart_holder>(m, "Permission")
        .def(py::init<std::string, std::optional<std::string>, std::optional<PermissionDefault>,
                      std::optional<std::unordered_map<std::string, bool>>>(),
             py::arg("name"), py::arg("description") = std::nullopt, py::arg("default_value") = std::nullopt,
             py::arg("children") = std::nullopt)
        .def_property_readonly("name", &Permission::getName)
        .def_property("description", &Permission::getDescription, &Permission::setDescription)
        .def_property("default", &Permission::getDefault, &Permission::setDefault)
        .def_property_readonly("children", &Permission::getChildren, py::return_value_policy::reference_internal)
        .def_property_readonly("permissibles", &Permission::getPermissibles)
        .def("recalculate_permissibles", &Permission::recalculatePermissibles)
        .def("addParent", py::overload_cast<const std::string &, bool>(&Permission::addParent));
}
