

# File permission.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**permissions**](dir_33a21cc2f228e5ad6b7d1bc8d0d1e9bc.md) **>** [**permission.h**](permission_8h.md)

[Go to the documentation of this file](permission_8h.md)


```C++
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

    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    std::unordered_map<std::string, bool> &getChildren()
    {
        return children_;
    }

    [[nodiscard]] PermissionDefault getDefault() const
    {
        return default_value_;
    }

    void setDefault(PermissionDefault value)
    {
        default_value_ = value;
        recalculatePermissibles();
    }

    [[nodiscard]] std::string getDescription() const
    {
        return description_;
    }

    void setDescription(std::string value)
    {
        description_ = std::move(value);
    }

    [[nodiscard]] std::unordered_set<Permissible *> getPermissibles() const
    {
        if (!plugin_manager_) {
            return {};
        }

        return plugin_manager_->getPermissionSubscriptions(name_);
    }

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
```


