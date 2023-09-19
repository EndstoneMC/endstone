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

class Permission {
public:
    inline const static PermissionDefault DefaultPermission = PermissionDefault::Operator;

    Permission(std::string name, std::string description, std::optional<PermissionDefault> default_value) noexcept
        : name_(std::move(name)), description_(std::move(description))
    {
        if (default_value.has_value()) {
            default_ = default_value.value();
        }
    }

    virtual ~Permission() noexcept = default;

    // Delete copy constructor and copy assignment operator
    Permission(const Permission &) = delete;
    Permission &operator=(const Permission &) = delete;

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

    [[nodiscard]] const std::unordered_map<std::string, PermissionDefault> &getChildren() const noexcept
    {
        return children_;
    }

    [[nodiscard]] std::vector<Permissible> getPermissibles() const noexcept
    {
        return getServer().getPluginManager().getPermissionSubscriptions(name_);
    }

    void recalculatePermissibles() noexcept
    {
        auto permissibles = getPermissibles();
        getServer().getPluginManager().recalculatePermissionDefaults(*this);

        for (auto &p : permissibles) {
            p.recalculatePermissions();
        }
    }

    [[nodiscard]] virtual Server &getServer() const noexcept = 0;

private:
    std::string name_;
    std::string description_;
    PermissionDefault default_ = DefaultPermission;
    std::unordered_map<std::string, PermissionDefault> children_;
};
