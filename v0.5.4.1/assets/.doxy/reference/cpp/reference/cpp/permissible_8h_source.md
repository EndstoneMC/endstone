

# File permissible.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**permissions**](dir_33a21cc2f228e5ad6b7d1bc8d0d1e9bc.md) **>** [**permissible.h**](permissible_8h.md)

[Go to the documentation of this file](permissible_8h.md)


```C++
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
#include <unordered_set>

#include "endstone/permissions/permission_attachment_info.h"

namespace endstone {

class CommandSender;
class Plugin;
class Permission;
class PermissionAttachment;

class Permissible {
public:
    virtual ~Permissible() = default;

    [[nodiscard]] virtual bool isOp() const = 0;

    virtual void setOp(bool value) = 0;

    [[nodiscard]] virtual bool isPermissionSet(std::string name) const = 0;

    [[nodiscard]] virtual bool isPermissionSet(const Permission &perm) const = 0;

    [[nodiscard]] virtual bool hasPermission(std::string name) const = 0;

    [[nodiscard]] virtual bool hasPermission(const Permission &perm) const = 0;

    virtual PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) = 0;

    virtual PermissionAttachment *addAttachment(Plugin &plugin) = 0;

    virtual bool removeAttachment(PermissionAttachment &attachment) = 0;

    virtual void recalculatePermissions() = 0;

    [[nodiscard]] virtual std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const = 0;

    [[nodiscard]] virtual CommandSender *asCommandSender() const = 0;
};
}  // namespace endstone
```


