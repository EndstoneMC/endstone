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

#include "endstone_python.h"

namespace py = pybind11;

namespace endstone::python {

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

void init_permissions(py::module_ &m, py_class<Permissible> &permissible, py::class_<Permission> &permission)
{
    permission  //
        .def(py::init(&createPermission), py::arg("name"), py::arg("description") = py::none(),
             py::arg("default") = py::none(), py::arg("children") = py::none())
        .def_property_readonly("name", &Permission::getName, "The unique fully qualified name of this `Permission`.")
        .def_property_readonly("children", &Permission::getChildren, py::return_value_policy::reference_internal,
                               R"doc(
    The children of this permission.

    If you change this map in any form, you must call `recalculate_permissibles()` to recalculate all `Permissibles`.
)doc")
        .def_property("default", &Permission::getDefault, &Permission::setDefault,
                      "The default value of this permission.")
        .def_property("description", &Permission::getDescription, &Permission::setDescription,
                      "A brief description of this permission.")
        .def_property_readonly("permissibles", &Permission::getPermissibles,
                               py::return_value_policy::reference_internal,
                               "A set containing every `Permissible` that has this permission.")
        .def("recalculate_permissibles", &Permission::recalculatePermissibles, R"doc(
    Recalculates all `Permissibles` that contain this permission.

    This should be called after modifying the children, and is automatically called after modifying the default value.
)doc")
        .def("add_parent", py::overload_cast<std::string, bool>(&Permission::addParent), py::arg("name"),
             py::arg("value"), R"doc(
    Adds this permission to the specified parent permission.

    If the parent permission does not exist, it will be created and registered.

    Args:
        name: Name of the parent permission.
        value: The value to set this permission to.

    Returns:
        The parent permission that was created or loaded.
)doc")
        .def("add_parent", py::overload_cast<Permission &, bool>(&Permission::addParent, py::const_), py::arg("perm"),
             py::arg("value"), R"doc(
    Adds this permission to the specified parent permission.

    Args:
        perm: Parent permission to register with.
        value: The value to set this permission to.
)doc");

    py::class_<PermissionAttachment>(m, "PermissionAttachment",
                                     "Holds information about a permission attachment on a `Permissible` object.")
        .def(py::init<Plugin &, Permissible &>(), py::arg("plugin"), py::arg("permissible"))
        .def_property_readonly("plugin", &PermissionAttachment::getPlugin, py::return_value_policy::reference,
                               "The plugin responsible for this permission attachment.")
        .def("remove", &PermissionAttachment::remove, R"doc(
    Removes this attachment from its registered `Permissible`.

    Returns:
        `True` if the attachment was removed successfully, `False` if it did not exist.
)doc")
        .def_property_readonly("permissible", &PermissionAttachment::getPermissible, py::return_value_policy::reference,
                               "The `Permissible` that this is attached to.")
        .def_property_readonly("permissions", &PermissionAttachment::getPermissions,
                               py::return_value_policy::reference_internal, R"doc(
    A copy of all set permissions and values contained within this attachment.

    This map may be modified but will not affect the attachment, as it is a copy.
)doc")
        .def("set_permission", py::overload_cast<std::string, bool>(&PermissionAttachment::setPermission),
             py::arg("name"), py::arg("value"), R"doc(
    Sets a permission to the given value, by its fully qualified name.

    Args:
        name: Name of the permission.
        value: New value of the permission.
)doc")
        .def("set_permission", py::overload_cast<Permission &, bool>(&PermissionAttachment::setPermission),
             py::arg("perm"), py::arg("value"), R"doc(
    Sets a permission to the given value.

    Args:
        perm: Permission to set.
        value: New value of the permission.
)doc")
        .def("unset_permission", py::overload_cast<std::string>(&PermissionAttachment::unsetPermission),
             py::arg("name"), R"doc(
    Removes the specified permission from this attachment.

    If the permission does not exist in this attachment, nothing will happen.

    Args:
        name: Name of the permission to remove.
)doc")
        .def("unset_permission", py::overload_cast<Permission &>(&PermissionAttachment::unsetPermission),
             py::arg("perm"), R"doc(
    Removes the specified permission from this attachment.

    If the permission does not exist in this attachment, nothing will happen.

    Args:
        perm: Permission to remove.
)doc")
        .def_property("removal_callback", &PermissionAttachment::getRemovalCallback,
                      &PermissionAttachment::setRemovalCallback,
                      "The callback to be called when this attachment is removed. May be empty.");

    py::class_<PermissionAttachmentInfo>(
        m, "PermissionAttachmentInfo",
        "Holds information on a permission and which `PermissionAttachment` provides it.")
        .def(py::init<Permissible &, std::string, PermissionAttachment *, bool>(), py::arg("permissible"),
             py::arg("permission"), py::arg("attachment"), py::arg("value"))
        .def_property_readonly("permissible", &PermissionAttachmentInfo::getPermissible,
                               py::return_value_policy::reference, "The `Permissible` this permission is for.")
        .def_property_readonly("permission", &PermissionAttachmentInfo::getPermission,
                               "The name of the permission being set.")
        .def_property_readonly("attachment", &PermissionAttachmentInfo::getAttachment,
                               py::return_value_policy::reference, R"doc(
    The attachment providing this permission.

    This may be `None` for default permissions (usually parent permissions).
)doc")
        .def_property_readonly("value", &PermissionAttachmentInfo::getValue, "The value of this permission.");

    permissible
        .def_property_readonly("permission_level", &Permissible::getPermissionLevel,
                               "The permission level of this object.")
        .def("is_permission_set", py::overload_cast<std::string>(&Permissible::isPermissionSet, py::const_),
             py::arg("name"), R"doc(
    Checks if this object contains an override for the specified permission, by fully qualified name.

    Args:
        name: Name of the permission.

    Returns:
        `True` if the permission is set, `False` otherwise.
)doc")
        .def("is_permission_set", py::overload_cast<const Permission &>(&Permissible::isPermissionSet, py::const_),
             py::arg("perm"), R"doc(
    Checks if this object contains an override for the specified `Permission`.

    Args:
        perm: Permission to check.

    Returns:
        `True` if the permission is set, `False` otherwise.
)doc")
        .def("has_permission", py::overload_cast<std::string>(&Permissible::hasPermission, py::const_), py::arg("name"),
             R"doc(
    Gets the value of the specified permission, if set.

    If a permission override is not set on this object, the default value of the permission will be returned.

    Args:
        name: Name of the permission.

    Returns:
        Value of the permission.
)doc")
        .def("has_permission", py::overload_cast<const Permission &>(&Permissible::hasPermission, py::const_),
             py::arg("perm"), R"doc(
    Gets the value of the specified permission, if set.

    If a permission override is not set on this object, the default value of the permission will be returned.

    Args:
        perm: Permission to get.

    Returns:
        Value of the permission.
)doc")
        .def("add_attachment", py::overload_cast<Plugin &, const std::string &, bool>(&Permissible::addAttachment),
             py::arg("plugin"), py::arg("name"), py::arg("value"), py::return_value_policy::reference, R"doc(
    Adds a new `PermissionAttachment` with a single permission by name and value.

    Args:
        plugin: Plugin responsible for this attachment; must not be disabled.
        name: Name of the permission to attach.
        value: Value of the permission.

    Returns:
        The `PermissionAttachment` that was just created.
)doc")
        .def("add_attachment", py::overload_cast<Plugin &>(&Permissible::addAttachment), py::arg("plugin"),
             py::return_value_policy::reference, R"doc(
    Adds a new empty `PermissionAttachment` to this object.

    Args:
        plugin: Plugin responsible for this attachment; must not be disabled.

    Returns:
        The `PermissionAttachment` that was just created.
)doc")
        .def("remove_attachment", &Permissible::removeAttachment, py::arg("attachment"), R"doc(
    Removes the given `PermissionAttachment` from this object.

    Args:
        attachment: Attachment to remove.

    Returns:
        `True` if the specified attachment was removed successfully, `False` when it isn't part of this object.
)doc")
        .def("recalculate_permissions", &Permissible::recalculatePermissions, R"doc(
    Recalculates the permissions for this object, if the attachments have changed values.

    This should very rarely need to be called from a plugin.
)doc")
        .def_property_readonly("effective_permissions", &Permissible::getEffectivePermissions,
                               py::return_value_policy::reference_internal,
                               "A set containing all the permissions currently in effect by this object.");
}

}  // namespace endstone::python
