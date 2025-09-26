

# File permission\_attachment\_info.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**permissions**](dir_33a21cc2f228e5ad6b7d1bc8d0d1e9bc.md) **>** [**permission\_attachment\_info.h**](permission__attachment__info_8h.md)

[Go to the documentation of this file](permission__attachment__info_8h.md)


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
#include <utility>

namespace endstone {

class Permissible;
class PermissionAttachment;

class PermissionAttachmentInfo {

public:
    PermissionAttachmentInfo(Permissible &permissible, std::string permission, PermissionAttachment *attachment,
                             bool value)
        : permissible_(permissible), permission_(std::move(permission)), attachment_(attachment), value_(value)
    {
    }

    [[nodiscard]] Permissible &getPermissible() const
    {
        return permissible_;
    }

    [[nodiscard]] std::string getPermission() const
    {
        return permission_;
    }

    [[nodiscard]] PermissionAttachment *getAttachment() const
    {
        return attachment_;
    }

    [[nodiscard]] bool getValue() const
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
```


