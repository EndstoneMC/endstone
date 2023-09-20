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

#include "endstone/permission/permissible_base.h"

#include <algorithm>
#include <unordered_map>

#include "endstone/endstone_server.h"
#include "endstone/permission/permission.h"
#include "endstone/permission/permission_attachment.h"
#include "endstone/server.h"

bool PermissibleBase::isPermissionSet(const std::string &name) const noexcept
{
    auto lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return permissions_.find(lower_name) != permissions_.end();
}

bool PermissibleBase::hasPermission(const std::string &name) const noexcept
{
    auto lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (isPermissionSet(lower_name)) {
        return permissions_.find(lower_name)->second.isGranted();
    }

    auto *permission = EndstoneServer::getInstance().getPluginManager().getPermission(lower_name);

    if (permission) {
        return permission->getDefault().isGrantedFor(*this);
    }
    return Permission::DefaultPermission.isGrantedFor(*this);
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin &plugin, const std::string &name, bool value) noexcept
{
    if (name.empty()) {
        EndstoneServer::getInstance().getLogger().error("Permission name cannot be empty.");
        return nullptr;
    }

    auto *result = addAttachment(plugin);
    if (result) {
        result->setPermission(name, value);
        recalculatePermissions();
    }

    return result;
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin &plugin) noexcept
{
    if (!plugin.isEnabled()) {
        EndstoneServer::getInstance().getLogger().error("Plugin {} is disabled.",
                                                        plugin.getDescription().getFullName());
        return nullptr;
    }

    auto result = std::make_unique<PermissionAttachment>(plugin, owner_);
    attachments_.push_back(std::move(result));
    recalculatePermissions();
    return attachments_.back().get();
}

bool PermissibleBase::removeAttachment(const PermissionAttachment &attachment) noexcept
{
    auto it = std::remove_if(attachments_.begin(), attachments_.end(), [&attachment](const auto &item) {
        return item.get() == &attachment;
    });

    if (it != attachments_.end()) {
        // Reclaim ownership of the unique_ptr before erasing it
        std::unique_ptr<PermissionAttachment> att = std::move(*it);

        attachments_.erase(attachments_.end());

        auto callback = att->getRemovalCallback();
        if (callback) {
            callback(*att);
        }

        recalculatePermissions();
        return true;
    }

    EndstoneServer::getInstance().getLogger().error("Given attachment is not part of Permissible object.");
    return false;
}

void PermissibleBase::recalculatePermissions() noexcept
{
    clearPermissions();
    auto &plugin_manager = EndstoneServer::getInstance().getPluginManager();
    plugin_manager.subscribeToDefaultPermissions(owner_);

    auto defaults = plugin_manager.getDefaultPermissions(getRole());
    for (auto &perm : defaults) {
        auto name = perm->getName();
        permissions_.insert_or_assign(name, PermissionAttachmentInfo(owner_, name, nullptr, true));
        plugin_manager.subscribeToPermission(name, owner_);
        calculateChildPermissions(perm->getChildren(), false, nullptr);
    }

    for (auto &attachment : attachments_) {
        calculateChildPermissions(attachment->getPermissions(), false, nullptr);
    }
}

void PermissibleBase::calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                                PermissionAttachment *attachment) noexcept
{
    auto &plugin_manager = EndstoneServer::getInstance().getPluginManager();

    for (const auto &entry : children) {
        std::string lower_name = entry.first;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        Permission *perm = plugin_manager.getPermission(lower_name);
        bool value = entry.second ^ invert;

        permissions_.insert_or_assign(lower_name, PermissionAttachmentInfo(owner_, lower_name, attachment, value));
        plugin_manager.subscribeToPermission(lower_name, owner_);

        if (perm) {
            calculateChildPermissions(perm->getChildren(), !value, attachment);
        }
    }
}
