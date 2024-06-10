

# File level.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**level.h**](level_8h.md)

[Go to the documentation of this file](level_8h.md)


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

#include "endstone/actor/actor.h"

namespace endstone {

class Level {
public:
    virtual ~Level() = default;

    [[nodiscard]] virtual std::string getName() const = 0;

    [[nodiscard]] virtual std::vector<Actor *> getActors() const = 0;

    [[nodiscard]] virtual int getTime() const = 0;

    virtual void setTime(int time) = 0;

    [[nodiscard]] virtual std::vector<Dimension *> getDimensions() const = 0;

    [[nodiscard]] virtual Dimension *getDimension(std::string name) const = 0;
};

}  // namespace endstone
```


