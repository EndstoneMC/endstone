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
#include <utility>

#include "endstone/util/pointers.h"

namespace endstone {

class Permissible;
class PermissionAttachment;

/**
 * Holds information on a permission and which PermissionAttachment provides it.
 */
class PermissionAttachmentInfo {

public:
    PermissionAttachmentInfo(const NotNull<Permissible> &permissible, std::string permission,
                             Nullable<PermissionAttachment> attachment, bool value)
        : permissible_(permissible.get()), permission_(std::move(permission)), attachment_(std::move(attachment)),
          value_(value)
    {
    }

    /**
     * Gets the permissible this is attached to.
     *
     * @return Permissible this permission is for, or nullptr if it no longer exists
     */
    [[nodiscard]] Nullable<Permissible> getPermissible() const { return permissible_.lock(); }

    /**
     * Gets the permission being set.
     *
     * @return Name of the permission
     */
    [[nodiscard]] std::string getPermission() const { return permission_; }

    /**
     * Gets the attachment providing this permission. This may be nullptr for default permissions (usually parent
     * permissions).
     *
     * @return Attachment
     */
    [[nodiscard]] Nullable<PermissionAttachment> getAttachment() const { return attachment_; }

    /**
     * Gets the value of this permission.
     *
     * @return Value of the permission
     */
    [[nodiscard]] bool getValue() const { return value_; }

private:
    std::weak_ptr<Permissible> permissible_;
    std::string permission_;
    Nullable<PermissionAttachment> attachment_;
    bool value_;
};

}  // namespace endstone
