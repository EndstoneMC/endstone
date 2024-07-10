

# File permission\_attachment.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**permissions**](dir_33a21cc2f228e5ad6b7d1bc8d0d1e9bc.md) **>** [**permission\_attachment.h**](permission__attachment_8h.md)

[Go to the documentation of this file](permission__attachment_8h.md)


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

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission.h"

namespace endstone {

using PermissionRemovedExecutor = std::function<void(const PermissionAttachment &)>;

class PermissionAttachment {
public:
    PermissionAttachment(Plugin &plugin, Permissible &permissible) : permissible_(permissible), plugin_(plugin) {}

    [[nodiscard]] Plugin &getPlugin() const
    {
        return plugin_;
    }

    void setRemovalCallback(PermissionRemovedExecutor ex)
    {
        removed_ = std::move(ex);
    }

    [[nodiscard]] PermissionRemovedExecutor getRemovalCallback() const
    {
        return removed_;
    }

    [[nodiscard]] Permissible &getPermissible() const
    {
        return permissible_;
    }

    [[nodiscard]] std::unordered_map<std::string, bool> getPermissions() const
    {
        return permissions_;
    }

    void setPermission(std::string name, bool value)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_[name] = value;
        permissible_.recalculatePermissions();
    }

    void setPermission(Permission &perm, bool value)
    {
        setPermission(perm.getName(), value);
    }

    void unsetPermission(std::string name)
    {
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
        permissions_.erase(name);
        permissible_.recalculatePermissions();
    }

    void unsetPermission(Permission &perm)
    {
        unsetPermission(perm.getName());
    }

    bool remove()
    {
        return permissible_.removeAttachment(*this);
    }

private:
    PermissionRemovedExecutor removed_;
    std::unordered_map<std::string, bool> permissions_;
    Permissible &permissible_;
    Plugin &plugin_;
};

}  // namespace endstone
```


