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

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "endstone/endstone_server.h"
#include "endstone/permission/permissible.h"
#include "endstone/permission/permission.h"
#include "endstone/permission/permission_attachment.h"
#include "endstone/server.h"

class PermissibleBase : public Permissible {
public:
    explicit PermissibleBase(Permissible &owner) : owner_(owner) {}

    // Delete copy constructor and copy assignment operator
    PermissibleBase(const PermissibleBase &) = delete;
    PermissibleBase &operator=(const PermissibleBase &) = delete;

    [[nodiscard]] PermissibleRole getRole() const noexcept override
    {
        return owner_.getRole();
    }

    [[nodiscard]] bool setRole(PermissibleRole role) const noexcept override
    {
        return owner_.setRole(role);
    }

    [[nodiscard]] bool isPermissionSet(const std::string &name) const noexcept override;
    [[nodiscard]] bool hasPermission(const std::string &name) const noexcept override;
    [[nodiscard]] PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name,
                                                      bool value) noexcept override;

    [[nodiscard]] PermissionAttachment *addAttachment(Plugin &plugin) noexcept override;
    [[nodiscard]] bool removeAttachment(const PermissionAttachment &attachment) noexcept override;
    void recalculatePermissions() noexcept override;

    void clearPermissions() noexcept
    {
        auto &plugin_manager = EndstoneServer::getInstance().getPluginManager();

        for (const auto &[name, permission] : permissions_) {
            plugin_manager.unsubscribeFromPermission(name, owner_);
        }

        plugin_manager.unsubscribeFromDefaultPermissions(owner_);
    }

private:
    void calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                   PermissionAttachment *attachment) noexcept;

    Permissible &owner_;
    std::unordered_map<std::string, PermissionAttachmentInfo> permissions_;
    std::vector<std::unique_ptr<PermissionAttachment>> attachments_;
};
