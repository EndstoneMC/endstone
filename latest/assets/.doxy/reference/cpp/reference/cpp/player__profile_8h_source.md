

# File player\_profile.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**profile**](dir_69aa83db54d47362b78b719d1ebae01d.md) **>** [**player\_profile.h**](player__profile_8h.md)

[Go to the documentation of this file](player__profile_8h.md)


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
#include <memory>
#include <future>
#include <optional>
#include "endstone/util/uuid.h"

namespace endstone{
class PlayerProfile {
public:
    virtual ~PlayerProfile() = default;

    virtual std::optional<UUID> getUniqueId() const = 0;

    virtual std::optional<std::string> getName() const = 0;

    virtual std::optional<std::string> getXuid() const = 0;

    virtual bool isComplete() const = 0;
};
}
```


