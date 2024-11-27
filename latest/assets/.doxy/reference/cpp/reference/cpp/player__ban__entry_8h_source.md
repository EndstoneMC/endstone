

# File player\_ban\_entry.h

[**File List**](files.md) **>** [**ban**](dir_f1b1f2e9abb31749ef58cd98f22bcd78.md) **>** [**player\_ban\_entry.h**](player__ban__entry_8h.md)

[Go to the documentation of this file](player__ban__entry_8h.md)


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

#include <utility>

#include "endstone/ban/ban_entry.h"
#include "endstone/util/uuid.h"

namespace endstone {

class PlayerBanEntry : public BanEntry {
public:
    explicit PlayerBanEntry(std::string name, std::optional<UUID> uuid = std::nullopt,
                            std::optional<std::string> xuid = std::nullopt)
        : name_(std::move(name)), uuid_(uuid), xuid_(std::move(xuid))
    {
    }

    [[nodiscard]] std::string getName() const
    {
        return name_;
    }

    [[nodiscard]] std::optional<UUID> getUniqueId() const
    {
        return uuid_;
    }

    [[nodiscard]] std::optional<std::string> getXuid() const
    {
        return xuid_;
    }

private:
    std::string name_;
    std::optional<UUID> uuid_;
    std::optional<std::string> xuid_;
};

}  // namespace endstone
```


