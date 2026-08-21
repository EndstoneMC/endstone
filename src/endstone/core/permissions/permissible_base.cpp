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
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include <entt/locator/locator.hpp>

#include "endstone/check.h"
#include "endstone/core/server.h"
#include "endstone/core/type.h"
#include "endstone/permissions/permission.h"
#include "endstone/permissions/permission_attachment_info.h"

namespace endstone::core {

PermissibleBase::PermissibleBase(Permissible *opable) : opable_(opable) {}

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

bool PermissibleBase::isPermissionSet(const NotNull<Permission> &perm) const
{
    return isPermissionSet(perm->getName());
}

bool PermissibleBase::hasPermission(std::string name) const
{
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (isPermissionSet(name)) {
        return permissions_.find(name)->second->getValue();
    }

    if (const auto perm = getPluginManager()->getPermission(name)) {
        return hasPermission(perm->getDefault(), getPermissionLevel());
    }
    return hasPermission(Permission::DefaultPermission, getPermissionLevel());
}

bool PermissibleBase::hasPermission(const NotNull<Permission> &perm) const
{
    auto name = perm->getName();
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (isPermissionSet(name)) {
        return permissions_.find(name)->second->getValue();
    }
    return hasPermission(perm->getDefault(), getPermissionLevel());
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

NotNull<PermissionAttachment> PermissibleBase::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    Preconditions::checkArgument(!name.empty(), "Could not add PermissionAttachment: permission name cannot be empty.");

    auto result = addAttachment(plugin);
    result->setPermission(name, value);
    recalculatePermissions();
    return result;
}

NotNull<PermissionAttachment> PermissibleBase::addAttachment(Plugin &plugin)
{
    Preconditions::checkArgument(plugin.isEnabled(), "Could not add PermissionAttachment: plugin {} is disabled.",
                                 plugin.getName());

    const auto &result = attachments_.emplace_back(std::make_shared<PermissionAttachment>(plugin, getParent()));
    recalculatePermissions();
    return result;
}

bool PermissibleBase::removeAttachment(const NotNull<PermissionAttachment> &attachment)
{
    const auto it = std::ranges::find(attachments_, attachment);
    if (it == attachments_.end()) {
        return false;
    }

    const auto removed = *it;
    attachments_.erase(it);
    if (const auto callback = removed->getRemovalCallback()) {
        callback(removed);
    }
    recalculatePermissions();
    return true;
}

void PermissibleBase::recalculatePermissions()
{
    clearPermissions();
    const auto parent = getParent();
    auto *plugin_manager = getPluginManager();
    const auto defaults = plugin_manager->getDefaultPermissions(getPermissionLevel());
    plugin_manager->subscribeToDefaultPerms(getPermissionLevel(), parent);

    for (const auto &perm : defaults) {
        auto name = perm->getName();
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_.insert_or_assign(name, std::make_shared<PermissionAttachmentInfo>(parent, name, nullptr, true));
        plugin_manager->subscribeToPermission(name, parent);
        calculateChildPermissions(perm->getChildren(), false, nullptr);
    }

    for (const auto &attachment : attachments_) {
        calculateChildPermissions(attachment->getPermissions(), false, attachment);
    }
}

// NOLINTNEXTLINE(*-no-recursion)
void PermissibleBase::calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                                const Nullable<PermissionAttachment> &attachment)
{
    const auto parent = getParent();
    for (const auto &entry : children) {
        auto name = entry.first;

        const auto perm = getPluginManager()->getPermission(name);
        std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
        const bool value = entry.second ^ invert;

        permissions_.insert_or_assign(name,
                                      std::make_shared<PermissionAttachmentInfo>(parent, name, attachment, value));
        getPluginManager()->subscribeToPermission(name, parent);

        if (perm) {
            calculateChildPermissions(perm->getChildren(), !value, attachment);
        }
    }
}

std::unordered_set<NotNull<PermissionAttachmentInfo>> PermissibleBase::getEffectivePermissions() const
{
    std::unordered_set<NotNull<PermissionAttachmentInfo>> result;
    for (const auto &entry : permissions_) {
        result.insert(entry.second);
    }
    return result;
}

const std::type_info &PermissibleBase::getClassTypeId() const
{
    return typeid(Permissible);
}

bool PermissibleBase::isInstanceOf(const std::type_info &target) const
{
    return core::isInstanceOf(*this, target);
}

void PermissibleBase::clearPermissions()
{
    auto *plugin_manager = getPluginManager();
    if (const auto parent = tryGetParent(); plugin_manager && parent) {
        const NotNull<Permissible> handle = parent;
        for (const auto &name : permissions_ | std::views::keys) {
            plugin_manager->unsubscribeFromPermission(name, handle);
        }
        plugin_manager->unsubscribeFromDefaultPerms(PermissionLevel::Default, handle);
        plugin_manager->unsubscribeFromDefaultPerms(PermissionLevel::Operator, handle);
        plugin_manager->unsubscribeFromDefaultPerms(PermissionLevel::Console, handle);
    }
    permissions_.clear();
}

Nullable<Permissible> PermissibleBase::tryGetParent() const
{
    auto *owner = opable_ != nullptr ? opable_ : const_cast<PermissibleBase *>(this);
    return owner->weak_from_this().lock();
}

NotNull<Permissible> PermissibleBase::getParent() const
{
    const auto parent = tryGetParent();
    Preconditions::checkState(parent != nullptr,
                              "Permissible is not owned by a std::shared_ptr, its permissions cannot be tracked.");
    return parent;
}

PluginManager *PermissibleBase::getPluginManager()
{
    if (entt::locator<EndstoneServer>::has_value()) {
        return &EndstoneServer::getInstance().getPluginManager();
    }
    return nullptr;
}

}  // namespace endstone::core
