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

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <nonstd/expected.hpp>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission_attachment.h"
#include "endstone/permissions/permission_attachment_info.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/permissions/permission_level.h"
#include "endstone/util/pointers.h"

namespace endstone::core {

/**
 * Base Permissible for use in any Permissible object via proxy or extension
 */
class PermissibleBase : public Permissible {
public:
    explicit PermissibleBase(Permissible *opable);

    [[nodiscard]] PermissionLevel getPermissionLevel() const override;
    [[nodiscard]] bool isPermissionSet(std::string name) const override;
    [[nodiscard]] bool isPermissionSet(const NotNull<Permission> &perm) const override;
    [[nodiscard]] bool hasPermission(std::string name) const override;
    [[nodiscard]] bool hasPermission(const NotNull<Permission> &perm) const override;
    NotNull<PermissionAttachment> addAttachment(Plugin &plugin, const std::string &name, bool value) override;
    NotNull<PermissionAttachment> addAttachment(Plugin &plugin) override;
    bool removeAttachment(const NotNull<PermissionAttachment> &attachment) override;
    void recalculatePermissions() override;
    [[nodiscard]] std::unordered_set<NotNull<PermissionAttachmentInfo>> getEffectivePermissions() const override;
    [[nodiscard]] const std::type_info &getClassTypeId() const override;
    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override;
    void clearPermissions();

private:
    [[nodiscard]] static PluginManager *getPluginManager();
    [[nodiscard]] Nullable<Permissible> tryGetParent() const;
    [[nodiscard]] NotNull<Permissible> getParent() const;
    void calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                   const Nullable<PermissionAttachment> &attachment);
    [[nodiscard]] static bool hasPermission(PermissionDefault default_value, PermissionLevel level);
    Permissible *opable_;
    std::vector<NotNull<PermissionAttachment>> attachments_;
    std::unordered_map<std::string, NotNull<PermissionAttachmentInfo>> permissions_;
};
}  // namespace endstone::core
