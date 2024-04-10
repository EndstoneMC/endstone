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

#pragma once

#include <string>
#include <unordered_set>

#include "endstone/permissions/permission_attachment_info.h"

namespace endstone {

class Plugin;
class Permission;
class PermissionAttachment;

/**
 * Represents an object that may become a server operator and can be assigned permissions.
 */
class Permissible {
public:
    virtual ~Permissible() = default;

    /**
     * @brief Checks if this object is a server operator
     *
     * @return true if this is an operator, otherwise false
     */
    [[nodiscard]] virtual bool isOp() const = 0;

    /**
     * @brief Sets the operator status of this object
     *
     * @param value New operator value
     */
    virtual void setOp(bool value) = 0;

    /**
     * Checks if this object contains an override for the specified permission, by fully qualified name
     *
     * @param name Name of the permission
     * @return true if the permission is set, otherwise false
     */
    [[nodiscard]] virtual bool isPermissionSet(std::string name) const = 0;

    /**
     * Checks if this object contains an override for the specified Permission
     *
     * @param perm Permission to check
     * @return true if the permission is set, otherwise false
     */
    [[nodiscard]] virtual bool isPermissionSet(const Permission &perm) const = 0;

    /**
     * Gets the value of the specified permission, if set.
     * If a permission override is not set on this object, the default value of the permission will be returned.
     *
     * @param name Name of the permission
     * @return Value of the permission
     */
    [[nodiscard]] virtual bool hasPermission(std::string name) const = 0;

    /**
     * Gets the value of the specified permission, if set.
     * If a permission override is not set on this object, the default value of the permission will be returned.
     *
     * @param perm Permission to get
     * @return Value of the permission
     */
    [[nodiscard]] virtual bool hasPermission(const Permission &perm) const = 0;

    /**
     * Adds a new PermissionAttachment with a single permission by name and value
     *
     * @param plugin Plugin responsible for this attachment, may not be null or disabled
     * @param name Name of the permission to attach
     * @param value Value of the permission
     * @return The PermissionAttachment that was just created
     */
    virtual PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) = 0;

    /**
     * Adds a new empty PermissionAttachment to this object
     *
     * @param plugin Plugin responsible for this attachment, may not be null or disabled
     * @return The PermissionAttachment that was just created
     */
    virtual PermissionAttachment *addAttachment(Plugin &plugin) = 0;

    /**
     * Removes the given PermissionAttachment from this object
     *
     * @param attachment Attachment to remove
     * @return true if the specified attachment was removed successfully, false when it isn't part of this object
     */
    virtual bool removeAttachment(PermissionAttachment &attachment) = 0;

    /**
     * Recalculates the permissions for this object, if the attachments have changed values.
     * This should very rarely need to be called from a plugin.
     */
    virtual void recalculatePermissions() = 0;

    /**
     * Gets a set containing all of the permissions currently in effect by this object
     *
     * @return Set of currently effective permissions
     */
    [[nodiscard]] virtual std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const = 0;
};
}  // namespace endstone
