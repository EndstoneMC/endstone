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
#include <unordered_map>
#include <vector>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission_attachment.h"
#include "endstone/permissions/permission_attachment_info.h"
#include "endstone/permissions/permission_default.h"

namespace endstone::detail {

/**
 * Base Permissible for use in any Permissible object via proxy or extension
 */
class PermissibleBase : public Permissible {
public:
    explicit PermissibleBase(Permissible *opable) : opable_(opable), parent_(opable ? *opable : *this) {}

    [[nodiscard]] bool isOp() const override;
    void setOp(bool value) override;
    [[nodiscard]] bool isPermissionSet(std::string name) const override;
    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override;
    [[nodiscard]] bool hasPermission(std::string name) const override;
    [[nodiscard]] bool hasPermission(const Permission &perm) const override;
    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override;
    PermissionAttachment *addAttachment(Plugin &plugin) override;
    bool removeAttachment(PermissionAttachment &attachment) override;
    void recalculatePermissions() override;
    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override;

private:
    void calculateChildPermissions(const std::unordered_map<std::string, bool> &children, bool invert,
                                   PermissionAttachment *attachment);
    [[nodiscard]] static bool hasPermission(PermissionDefault default_value, bool op);
    Permissible *opable_;
    Permissible &parent_;
    std::vector<std::unique_ptr<PermissionAttachment>> attachments_{};
    std::unordered_map<std::string, std::unique_ptr<PermissionAttachmentInfo>> permissions_{};
};
}  // namespace endstone::detail
