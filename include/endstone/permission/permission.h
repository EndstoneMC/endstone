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

#pragma once

#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include "endstone/permission/permission_default.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"
#include "fmt/color.h"

class Permission {
public:
    inline const static PermissionDefault DefaultPermission = PermissionDefault::Operator;

    explicit Permission(std::string name, std::optional<std::string> description = std::nullopt,
                        std::optional<PermissionDefault> default_value = std::nullopt,
                        std::optional<std::unordered_map<std::string, bool>> children = std::nullopt) noexcept
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        name_ = name;

        if (description.has_value()) {
            description_ = description.value();
        }

        if (default_value.has_value()) {
            default_ = default_value.value();
        }

        if (children.has_value()) {
            children_ = children.value();
        }
    }

    virtual ~Permission() noexcept = default;

    // Delete copy constructor and copy assignment operator
    Permission(const Permission &) = delete;
    Permission &operator=(const Permission &) = delete;

    // Enable move constructor and move assignment operator
    Permission(Permission &&) noexcept = default;
    Permission &operator=(Permission &&) noexcept = default;

    [[nodiscard]] const std::string &getName() const noexcept
    {
        return name_;
    }

    [[nodiscard]] const std::string &getDescription() const noexcept
    {
        return description_;
    }

    void setDescription(const std::string &description) noexcept
    {
        description_ = description;
    }

    [[nodiscard]] const PermissionDefault &getDefault() const noexcept
    {
        return default_;
    }

    void setDefault(const PermissionDefault &default_value) noexcept
    {
        if (default_value != default_) {
            default_ = default_value;
            recalculatePermissibles();
        }
    }

    [[nodiscard]] std::unordered_map<std::string, bool> &getChildren() noexcept
    {
        return children_;
    }

    [[nodiscard]] std::vector<Permissible *> getPermissibles() const noexcept
    {
        if (!plugin_manager_) {
            return {};
        }

        return plugin_manager_->getPermissionSubscriptions(name_);
    }

    void recalculatePermissibles() noexcept
    {
        if (!plugin_manager_) {
            return;
        }

        auto permissibles = getPermissibles();
        plugin_manager_->recalculatePermissionDefaults(*this);

        for (auto &p : permissibles) {
            p->recalculatePermissions();
        }
    }

    Permission *addParent(const std::string &name, bool value) noexcept
    {
        if (!plugin_manager_) {
            return nullptr;
        }

        auto lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        auto *permission = plugin_manager_->getPermission(lower_name);
        if (!permission) {
            plugin_manager_->addPermission(lower_name);
            permission = plugin_manager_->getPermission(lower_name);
        }

        if (!permission) {
            return nullptr;
        }

        addParent(*permission, value);
        return permission;
    }

    void addParent(Permission &permission, bool value) const noexcept
    {
        permission.getChildren().insert({getName(), value});
        permission.recalculatePermissibles();
    }

private:
    friend class SimplePluginManager;

    std::string name_;
    std::string description_;
    PermissionDefault default_ = DefaultPermission;
    std::unordered_map<std::string, bool> children_;

    PluginManager *plugin_manager_;

    inline const static std::regex ValidName{"^[a-zA-Z_]+(\\.[a-zA-Z_]+)*$"};
};
