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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin_manager.h"

namespace endstone {

/**
 * Represents a unique permission that may be attached to a Permissible
 */
class Permission {
public:
    static const PermissionDefault DefaultPermission = PermissionDefault::Operator;

    explicit Permission(std::string name, std::string description = "",
                        PermissionDefault default_value = DefaultPermission,
                        std::unordered_map<std::string, bool> children = {})
    {
        this->name_ = std::move(name);
        this->description_ = std::move(description);
        this->default_value_ = default_value;
        this->children_ = std::move(children);
    }

    /**
     * Returns the unique fully qualified name of this Permission
     *
     * @return Fully qualified name
     */
    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    /**
     * Gets the children of this permission.
     * If you change this map in any form, you must call recalculatePermissibles() to recalculate all Permissibles
     *
     * @return Permission children
     */
    std::unordered_map<std::string, bool> &getChildren()
    {
        return children_;
    }

    /**
     * Gets the default value of this permission.
     *
     * @return Default value of this permission.
     */
    [[nodiscard]] PermissionDefault getDefault() const
    {
        return default_value_;
    }

    /**
     * Sets the default value of this permission.
     *
     * This will not be saved to disk, and is a temporary operation until the server reloads permissions.
     * Changing this default will cause all Permissibles that contain this permission to recalculate their permissions
     *
     * @param value The new default to set
     */
    void setDefault(PermissionDefault value)
    {
        default_value_ = value;
        recalculatePermissibles();
    }

    /**
     * Gets a brief description of this permission, may be empty
     *
     * @return Brief description of this permission
     */
    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    /**
     * Sets the description of this permission.
     *
     * This will not be saved to disk, and is a temporary operation until the server reloads permissions.
     *
     * @param value The new description to set
     */
    void setDescription(std::string value)
    {
        description_ = std::move(value);
    }

    /**
     * Gets a set containing every Permissible that has this permission.
     * This set cannot be modified.
     *
     * @return Set containing permissibles with this permission
     */
    [[nodiscard]] std::unordered_set<Permissible *> getPermissibles() const
    {
        if (!plugin_manager_) {
            return {};
        }

        return plugin_manager_->getPermissionSubscriptions(name_);
    }

    /**
     * Recalculates all Permissibles that contain this permission.
     *
     * This should be called after modifying the children, and is automatically called after modifying the default value
     */
    void recalculatePermissibles()
    {
        if (!plugin_manager_) {
            return;
        }

        auto perms = getPermissibles();
        plugin_manager_->recalculatePermissionDefaults(*this);

        for (auto *p : perms) {
            p->recalculatePermissions();
        }
    }

    /**
     * Adds this permission to the specified parent permission.
     *
     * If the parent permission does not exist, it will be created and registered.
     *
     * @param name Name of the parent permission
     * @param value The value to set this permission to
     * @return Parent permission it created or loaded
     */
    Permission *addParent(std::string name, bool value)
    {
        if (!plugin_manager_) {
            return nullptr;
        }

        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        auto *perm = plugin_manager_->getPermission(name);

        if (!perm) {
            perm = plugin_manager_->addPermission(std::make_unique<Permission>(name));
        }

        addParent(*perm, value);
        return perm;
    }

    /**
     * Adds this permission to the specified parent permission.
     *
     * @param perm Parent permission to register with
     * @param value The value to set this permission to
     */
    void addParent(Permission &perm, bool value) const
    {
        perm.getChildren()[getName()] = value;
        perm.recalculatePermissibles();
    }

    void init(PluginManager &plugin_manager)
    {
        plugin_manager_ = &plugin_manager;
    }

private:
    std::string name_;
    std::unordered_map<std::string, bool> children_;
    PermissionDefault default_value_ = DefaultPermission;
    std::string description_;
    PluginManager *plugin_manager_;
};

}  // namespace endstone
