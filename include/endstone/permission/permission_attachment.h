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

#include <functional>
#include <string>
#include <utility>

#include <fmt/color.h>

#include "endstone/permission/permissible.h"
#include "endstone/plugin/plugin.h"

class PermissionAttachment;

class PermissionAttachmentInfo {
public:
    PermissionAttachmentInfo(Permissible &permissible, std::string permission, PermissionAttachment *attachment,
                             bool value)
        : permissible_(&permissible), permission_(permission), attachment_(attachment), value_(value)
    {
        std::transform(permission.begin(), permission.end(), permission.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        permission_ = std::move(permission);
    }

    [[nodiscard]] Permissible &getPermissible() const noexcept
    {
        return *permissible_;
    }

    [[nodiscard]] std::string getPermission() const noexcept
    {
        return permission_;
    }

    [[nodiscard]] PermissionAttachment *getAttachment() const noexcept
    {
        return attachment_;
    }

    [[nodiscard]] bool isGranted() const noexcept
    {
        return value_;
    }

private:
    Permissible *permissible_;
    std::string permission_;
    PermissionAttachment *attachment_;
    bool value_;
};

using PermissionRemovedExecutor = std::function<void(PermissionAttachment &)>;

class PermissionAttachment {
public:
    PermissionAttachment(Plugin &plugin, Permissible &permissible) noexcept : permissible_(permissible), plugin_(plugin)
    {
    }

    [[nodiscard]] Plugin &getPlugin() const noexcept
    {
        return plugin_;
    }

    void setRemovalCallback(PermissionRemovedExecutor executor) noexcept
    {
        removal_callback_ = std::move(executor);
    }

    [[nodiscard]] PermissionRemovedExecutor getRemovalCallback() const noexcept
    {
        return removal_callback_;
    }

    [[nodiscard]] Permissible &getPermissible() const noexcept
    {
        return permissible_;
    }

    [[nodiscard]] std::unordered_map<std::string, bool> getPermissions() const noexcept
    {
        return permissions_;
    }

    void setPermission(const std::string &name, bool value) noexcept
    {
        std::string lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        permissions_[lower_name] = value;
        permissible_.recalculatePermissions();
    }

    void setPermission(Permission &perm, bool value) noexcept
    {
        setPermission(perm.getName(), value);
    }

    void unsetPermission(const std::string &name) noexcept
    {
        std::string lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        permissions_.erase(lower_name);
        permissible_.recalculatePermissions();
    }

    void unsetPermission(Permission &perm) noexcept
    {
        unsetPermission(perm.getName());
    }

    bool remove() noexcept
    {
        return permissible_.removeAttachment(*this);
    }

private:
    PermissionRemovedExecutor removal_callback_;
    std::unordered_map<std::string, bool> permissions_;
    Permissible &permissible_;
    Plugin &plugin_;
};
