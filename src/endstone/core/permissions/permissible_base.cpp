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

#include "endstone/core/permissions/permissible_base.h"

#include <memory>

#include "endstone/core/server.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_attachment_info.h"

namespace endstone::core {

PermissibleBase::PermissibleBase(Permissible *opable) : opable_(opable), parent_(opable ? *opable : *this) {}

PermissibleBase::~PermissibleBase()
{
    if (getPluginManager()) {
        // Ensure clearPermissions() is called during destruction to remove this object from the permission subscription
        // list and avoid dangling pointers. During server shutdown, if the plugin manager is already destroyed, this
        // step is safely skipped as no dangling pointers can occur.
        clearPermissions();
    }
}

PermissionLevel PermissibleBase::getPermissionLevel() const
{
    if (opable_) {
        return opable_->getPermissionLevel();
    }
    return PermissionLevel::Default;
}

bool PermissibleBase::isPermissionSet(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    return permissions_.contains(name);
}

bool PermissibleBase::isPermissionSet(const Permission &perm) const
{
    return isPermissionSet(perm.getName());
}

bool PermissibleBase::hasPermission(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (isPermissionSet(name)) {
        return permissions_.find(name)->second->getValue();
    }

    if (auto *perm = getPluginManager()->getPermission(name); perm != nullptr) {
        return hasPermission(perm->getDefault(), getPermissionLevel());
    }
    return hasPermission(Permission::DefaultPermission, getPermissionLevel());
}

bool PermissibleBase::hasPermission(const Permission &perm) const
{
    auto name = perm.getName();
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (isPermissionSet(name)) {
        return permissions_.find(name)->second->getValue();
    }
    return hasPermission(perm.getDefault(), getPermissionLevel());
}

bool PermissibleBase::hasPermission(PermissionDefault default_value, PermissionLevel level)
{
    switch (default_value) {
    case PermissionDefault::True:
        return true;
    case PermissionDefault::False:
        return false;
    case PermissionDefault::Operator:
        return level >= PermissionLevel::Operator;
    case PermissionDefault::NotOperator:
        return level == PermissionLevel::Default;
    case PermissionDefault::Console:
        return level == PermissionLevel::Console;
    default:
        return false;
    }
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    if (name.empty()) {
        plugin.getLogger().error("Could not add PermissionAttachment: Permission name cannot be empty");
        return nullptr;
    }

    auto *result = addAttachment(plugin);
    if (result) {
        result->setPermission(name, value);
        recalculatePermissions();
    }

    return result;
}

PermissionAttachment *PermissibleBase::addAttachment(Plugin &plugin)
{
    if (!plugin.isEnabled()) {
        plugin.getLogger().error("Could not add PermissionAttachment: Plugin is disabled");
        return nullptr;
    }

    const auto &it = attachments_.emplace_back(std::make_unique<PermissionAttachment>(plugin, parent_));
    auto *result = it.get();
    recalculatePermissions();
    return result;
}

Result<void> PermissibleBase::removeAttachment(PermissionAttachment &attachment)
{
    const auto it =
        std::ranges::find_if(attachments_, [&attachment](const auto &item) { return item.get() == &attachment; });

    if (it != attachments_.end()) {
        if (auto callback = it->get()->getRemovalCallback()) {
            callback(attachment);
        }

        attachments_.erase(it);
        recalculatePermissions();
        return {};
    }

    return nonstd::make_unexpected("Given attachment is not part of Permissible object.");
}

void PermissibleBase::recalculatePermissions()
{
    clearPermissions();
    const auto defaults = getPluginManager()->getDefaultPermissions(getPermissionLevel());
    getPluginManager()->subscribeToDefaultPerms(getPermissionLevel(), parent_);

    for (auto *perm : defaults) {
        auto name = perm->getName();
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_[name] = std::make_unique<PermissionAttachmentInfo>(parent_, name, nullptr, true);
        getPluginManager()->subscribeToPermission(name, parent_);
        calculateChildPermissions(perm->getChildren(), false, nullptr);
    }

    for (const auto &attachment : attachments_) {
        calculateChildPermissions(attachment->getPermissions(), false, attachment.get());
    }
}

// NOLINTNEXTLINE(*-no-recursion)
void PermissibleBase::calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                                PermissionAttachment *attachment)
{
    for (const auto &entry : children) {
        auto name = entry.first;

        auto *perm = getPluginManager()->getPermission(name);
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
        bool value = entry.second ^ invert;

        permissions_[name] = std::make_unique<PermissionAttachmentInfo>(parent_, name, attachment, value);
        getPluginManager()->subscribeToPermission(name, parent_);

        if (perm != nullptr) {
            calculateChildPermissions(perm->getChildren(), !value, attachment);
        }
    }
}

std::unordered_set<PermissionAttachmentInfo *> PermissibleBase::getEffectivePermissions() const
{
    std::unordered_set<PermissionAttachmentInfo *> result;
    for (const auto &entry : permissions_) {
        result.insert(entry.second.get());
    }
    return result;
}

CommandSender *PermissibleBase::asCommandSender() const
{
    return nullptr;
}

void PermissibleBase::clearPermissions()
{
    // Clear permissions
    for (const auto &[name, perm] : permissions_) {
        getPluginManager()->unsubscribeFromPermission(name, parent_);
    }
    getPluginManager()->unsubscribeFromDefaultPerms(PermissionLevel::Default, parent_);
    getPluginManager()->unsubscribeFromDefaultPerms(PermissionLevel::Operator, parent_);
    getPluginManager()->unsubscribeFromDefaultPerms(PermissionLevel::Console, parent_);
    permissions_.clear();
}

PluginManager *PermissibleBase::getPluginManager()
{
    if (entt::locator<EndstoneServer>::has_value()) {
        return &entt::locator<EndstoneServer>::value().getPluginManager();
    }
    return nullptr;
}

}  // namespace endstone::core
