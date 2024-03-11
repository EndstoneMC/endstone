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

#include <string>

namespace endstone {

class Permissible;
class PermissionAttachment;

/**
 * Holds information on a permission and which PermissionAttachment provides it
 */
class PermissionAttachmentInfo {

public:
    PermissionAttachmentInfo(Permissible &permissible, std::string permission, PermissionAttachment *attachment,
                             bool value)
        : permissible_(permissible), permission_(permission), attachment_(attachment), value_(value)
    {
    }

    /**
     * Gets the permissible this is attached to
     *
     * @return Permissible this permission is for
     */
    Permissible &getPermissible()
    {
        return permissible_;
    }

    /**
     * Gets the permission being set
     *
     * @return Name of the permission
     */
    std::string getPermission()
    {
        return permission_;
    }

    /**
     * Gets the attachment providing this permission. This may be null for default permissions (usually parent permissions).
     *
     * @return Attachment
     */
    PermissionAttachment *getAttachment()
    {
        return attachment_;
    }

    /**
     * Gets the value of this permission
     *
     * @return Value of the permission
     */
    bool getValue()
    {
        return value_;
    }

private:
    Permissible &permissible_;
    std::string permission_;
    PermissionAttachment *attachment_;
    bool value_;
};

}  // namespace endstone
