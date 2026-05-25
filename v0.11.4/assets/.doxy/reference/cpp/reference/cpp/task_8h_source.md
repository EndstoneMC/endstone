

# File task.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**scheduler**](dir_3a50fadda3c7cdc27c965a478a3f7d27.md) **>** [**task.h**](task_8h.md)

[Go to the documentation of this file](task_8h.md)


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

#include <cstdint>

#include "endstone/plugin/plugin.h"

namespace endstone {

using TaskId = std::uint32_t;

class Task {
public:
    virtual ~Task() = default;

    [[nodiscard]] virtual TaskId getTaskId() const = 0;

    [[nodiscard]] virtual Plugin *getOwner() const = 0;

    [[nodiscard]] virtual bool isSync() const = 0;

    [[nodiscard]] virtual bool isCancelled() const = 0;

    virtual void cancel() = 0;
};

}  // namespace endstone
```


