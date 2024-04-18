// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_attachment.h"
#include "endstone/permissions/permission_attachment_info.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin.h"

namespace py = pybind11;

namespace endstone::detail {

namespace {
Permission createPermission(std::string name, const std::optional<std::string> &description,
                            std::optional<PermissionDefault> default_value,
                            const std::optional<std::unordered_map<std::string, bool>> &children,
                            const py::args & /*args*/, const py::kwargs & /*kwargs*/)
{
    return Permission(std::move(name), description.value_or(""), default_value.value_or(Permission::DefaultPermission),
                      children.value_or(std::unordered_map<std::string, bool>{}));
}
}  // namespace

void init_permissions(py::module_ &m, py::class_<Permissible> &permissible, py::class_<Permission> &permission,
                      py::enum_<PermissionDefault> &permission_default)
{
    permission_default  //
        .value("TRUE", PermissionDefault::True)
        .value("FALSE", PermissionDefault::False)
        .value("OP", PermissionDefault::Operator)
        .value("OPERATOR", PermissionDefault::Operator)
        .value("NOT_OP", PermissionDefault::NotOperator)
        .value("NOT_OPERATOR", PermissionDefault::NotOperator);

    permission  //
        .def(py::init(&createPermission), py::arg("name"), py::arg("description") = py::none(),
             py::arg("default") = py::none(), py::arg("children") = py::none())
        .def_property_readonly("name", &Permission::getName, "Gets the unique fully qualified name of this Permission.")
        .def_property_readonly("children", &Permission::getChildren, py::return_value_policy::reference_internal,
                               "Gets the children of this permission.")
        .def_property("default", &endstone::Permission::getDefault, &endstone::Permission::setDefault,
                      "The default value of this permission.")
        .def_property("description", &endstone::Permission::getDescription, &endstone::Permission::setDescription,
                      "The brief description of this permission")
        .def_property_readonly("permissibles", &endstone::Permission::getPermissibles,
                               py::return_value_policy::reference_internal,
                               "Gets a set containing every Permissible that has this permission.")
        .def("recalculate_permissibles", &endstone::Permission::recalculatePermissibles,
             "Recalculates all Permissibles that contain this permission.")
        .def("add_parent", py::overload_cast<std::string, bool>(&endstone::Permission::addParent), py::arg("name"),
             py::arg("value"), "Adds this permission to the specified parent permission.")
        .def("add_parent", py::overload_cast<Permission &, bool>(&endstone::Permission::addParent, py::const_),
             py::arg("perm"), py::arg("value"), "Adds this permission to the specified parent permission.");

    py::class_<PermissionAttachment>(m, "PermissionAttachment")
        .def(py::init<Plugin &, Permissible &>(), py::arg("plugin"), py::arg("permissible"))
        .def_property_readonly("plugin", &PermissionAttachment::getPlugin, py::return_value_policy::reference,
                               "Gets the plugin responsible for this attachment.")
        .def("remove", &PermissionAttachment::remove, "Removes this attachment from its registered Permissible.")
        .def_property_readonly("permissible", &PermissionAttachment::getPermissible, py::return_value_policy::reference,
                               "Gets the Permissible that this is attached to.")
        .def_property_readonly("permissions", &PermissionAttachment::getPermissions,
                               py::return_value_policy::reference_internal,
                               "Gets a copy of all set permissions and values contained within this attachment.")
        .def("set_permission", py::overload_cast<std::string, bool>(&PermissionAttachment::setPermission),
             py::arg("name"), py::arg("value"), "Sets a permission to the given value, by its fully qualified name.")
        .def("set_permission", py::overload_cast<Permission &, bool>(&PermissionAttachment::setPermission),
             py::arg("perm"), py::arg("value"), "Sets a permission to the given value.")
        .def("unset_permission", py::overload_cast<std::string>(&PermissionAttachment::unsetPermission),
             py::arg("name"), "Removes the specified permission from this attachment by name.")
        .def("unset_permission", py::overload_cast<Permission &>(&PermissionAttachment::unsetPermission),
             py::arg("perm"), "Removes the specified permission from this attachment.")
        .def_property("removal_callback", &PermissionAttachment::getRemovalCallback,
                      &PermissionAttachment::setRemovalCallback,
                      "The callback to be called when this attachment is removed.");

    py::class_<PermissionAttachmentInfo>(m, "PermissionAttachmentInfo")
        .def(py::init<Permissible &, std::string, PermissionAttachment *, bool>(), py::arg("permissible"),
             py::arg("permission"), py::arg("attachment"), py::arg("value"))
        .def_property_readonly("permissible", &PermissionAttachmentInfo::getPermissible,
                               py::return_value_policy::reference, "Get the permissible this is attached to")
        .def_property_readonly("permission", &PermissionAttachmentInfo::getPermission, "Gets the permission being set")
        .def_property_readonly("attachment", &PermissionAttachmentInfo::getAttachment,
                               py::return_value_policy::reference, "Gets the attachment providing this permission.")
        .def_property_readonly("value", &PermissionAttachmentInfo::getValue, "Gets the value of this permission");

    permissible.def_property("op", &Permissible::isOp, &Permissible::setOp, "The operator status of this object")
        .def("is_permission_set", py::overload_cast<std::string>(&Permissible::isPermissionSet, py::const_),
             py::arg("name"), "Checks if a permissions is set by name.")
        .def("is_permission_set", py::overload_cast<const Permission &>(&Permissible::isPermissionSet, py::const_),
             py::arg("perm"), "Checks if a permissions is set by permission.")
        .def("has_permission", py::overload_cast<std::string>(&Permissible::hasPermission, py::const_), py::arg("name"),
             "Checks if a permissions is available by name.")
        .def("has_permission", py::overload_cast<const Permission &>(&Permissible::hasPermission, py::const_),
             py::arg("perm"), "Checks if a permissions is available by permission.")
        .def("add_attachment", py::overload_cast<Plugin &, const std::string &, bool>(&Permissible::addAttachment),
             py::arg("plugin"), py::arg("name"), py::arg("value"), py::return_value_policy::reference,
             "Adds a new PermissionAttachment.")
        .def("add_attachment", py::overload_cast<Plugin &>(&Permissible::addAttachment), py::arg("plugin"),
             py::return_value_policy::reference, "Adds a new PermissionAttachment.")
        .def("remove_attachment", &Permissible::removeAttachment, py::arg("attachment"),
             "Removes a given PermissionAttachment.")
        .def("recalculate_permissions", &Permissible::recalculatePermissions, "Recalculates the permissions.")
        .def_property_readonly("effective_permissions", &Permissible::getEffectivePermissions,
                               py::return_value_policy::reference_internal, "Gets effective permissions.");
}

}  // namespace endstone::detail
