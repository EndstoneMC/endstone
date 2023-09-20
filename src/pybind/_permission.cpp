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
#include "endstone/permission/permission_attachment.h"
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
        .def_property("default", &Permission::getDefault, &Permission::setDefault, py::return_value_policy::reference)
        .def_property_readonly("children", &Permission::getChildren, py::return_value_policy::reference_internal)
        .def_property_readonly("permissibles", &Permission::getPermissibles,
                               py::return_value_policy::reference_internal)
        .def("recalculate_permissibles", &Permission::recalculatePermissibles)
        .def("add_parent", py::overload_cast<const std::string &, bool>(&Permission::addParent),
             py::return_value_policy::reference)
        .def_readonly_static("DEFAULT_PERMISSION", &Permission::DefaultPermission);

    auto permissible =
        py::class_<Permissible>(m, "Permissible")
            .def_property("role", &Permissible::getRole, &Permissible::setRole)
            .def("is_permission_set", &Permissible::isPermissionSet)
            .def("has_permission", &Permissible::hasPermission)
            .def("add_attachment", py::overload_cast<Plugin &, const std::string &, bool>(&Permissible::addAttachment),
                 py::return_value_policy::reference)
            .def("add_attachment", py::overload_cast<Plugin &>(&Permissible::addAttachment),
                 py::return_value_policy::reference)
            .def("remove_attachment", &Permissible::removeAttachment)
            .def("recalculate_permissions", &Permissible::recalculatePermissions);

    py::enum_<PermissibleRole>(permissible, "PermissibleRole")
        .value("VISITOR", PermissibleRole::Visitor)
        .value("MEMBER", PermissibleRole::Member)
        .value("OPERATOR", PermissibleRole::Operator)
        .export_values();

    py::class_<PermissionDefault>(m, "PermissionDefault")
        .def("is_granted_for", py::overload_cast<const Permissible &>(&PermissionDefault::isGrantedFor, py::const_))
        .def("is_granted_for", py::overload_cast<PermissibleRole>(&PermissionDefault::isGrantedFor, py::const_))
        .def_static("get_by_name", &PermissionDefault::getByName, py::return_value_policy::reference)
        .def("__eq__", &PermissionDefault::operator==)
        .def("__ne__", &PermissionDefault::operator!=)
        .def_readonly_static("TRUE", &PermissionDefault::True)
        .def_readonly_static("FALSE", &PermissionDefault::False)
        .def_readonly_static("VISITOR", &PermissionDefault::Visitor)
        .def_readonly_static("NOT_VISITOR", &PermissionDefault::NotVisitor)
        .def_readonly_static("MEMBER", &PermissionDefault::Member)
        .def_readonly_static("NOT_MEMBER", &PermissionDefault::NotMember)
        .def_readonly_static("OPERATOR", &PermissionDefault::Operator)
        .def_readonly_static("NOT_OPERATOR", &PermissionDefault::NotOperator);
}
