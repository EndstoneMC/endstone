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

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission.h"

namespace endstone {

using PermissionRemovedExecutor = std::function<void(const PermissionAttachment &)>;

/**
 * Holds information about a permission attachment on a Permissible object
 */
class PermissionAttachment {
public:
    PermissionAttachment(Plugin &plugin, Permissible &permissible) : permissible_(permissible), plugin_(plugin) {}

    /**
     * Gets the plugin responsible for this attachment
     *
     * @return Plugin responsible for this permission attachment
     */
    [[nodiscard]] Plugin &getPlugin() const
    {
        return plugin_;
    }

    /**
     * Sets an executor to be called for when this attachment is removed from a Permissible. May be empty.
     *
     * @param ex Executor to be called when this is removed
     */
    void setRemovalCallback(PermissionRemovedExecutor ex)
    {
        removed_ = std::move(ex);
    }

    /**
     * Gets the class that was previously set to be called when this attachment was removed from a Permissible. May be
     * empty.
     *
     * @return Executor to be called when this is removed
     */
    [[nodiscard]] PermissionRemovedExecutor getRemovalCallback() const
    {
        return removed_;
    }

    /**
     * Gets the Permissible that this is attached to
     *
     * @return Permissible containing this attachment
     */
    [[nodiscard]] Permissible &getPermissible() const
    {
        return permissible_;
    }

    /**
     * Gets a copy of all set permissions and values contained within this attachment.
     * This map may be modified but will not affect the attachment, as it is a copy.
     *
     * @return Copy of all permissions and values expressed by this attachment
     */
    [[nodiscard]] std::unordered_map<std::string, bool> getPermissions() const
    {
        return permissions_;
    }

    /**
     * Sets a permission to the given value, by its fully qualified name
     *
     * @param name Name of the permission
     * @param value New value of the permission
     */
    void setPermission(std::string name, bool value)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_[name] = value;
        permissible_.recalculatePermissions();
    }

    /**
     * Sets a permission to the given value
     *
     * @param perm Permission to set
     * @param value New value of the permission
     */
    void setPermission(Permission &perm, bool value)
    {
        setPermission(perm.getName(), value);
    }

    /**
     * Removes the specified permission from this attachment.
     * If the permission does not exist in this attachment, nothing will happen.
     *
     * @param name Name of the permission to remove
     */
    void unsetPermission(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_.erase(name);
        permissible_.recalculatePermissions();
    }

    /**
     * Removes the specified permission from this attachment.
     * If the permission does not exist in this attachment, nothing will happen.
     *
     * @param perm Permission to remove
     */
    void unsetPermission(Permission &perm)
    {
        unsetPermission(perm.getName());
    }

    /**
     * Removes this attachment from its registered Permissible
     *
     * @return true if the permissible was removed successfully, false if it did not exist
     */
    bool remove()
    {
        return permissible_.removeAttachment(*this);
    }

private:
    PermissionRemovedExecutor removed_;
    std::unordered_map<std::string, bool> permissions_;
    Permissible &permissible_;
    Plugin &plugin_;
};

}  // namespace endstone
