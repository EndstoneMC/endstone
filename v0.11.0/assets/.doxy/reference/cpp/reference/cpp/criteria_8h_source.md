

# File criteria.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**scoreboard**](dir_19c52f9ea81a2cf7449c80dcee80d6f0.md) **>** [**criteria.h**](criteria_8h.md)

[Go to the documentation of this file](criteria_8h.md)


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

#include "endstone/scoreboard/render_type.h"

namespace endstone {

class Criteria {
public:
    enum class Type {
        Dummy,
    };

    virtual ~Criteria() = default;

    [[nodiscard]] virtual std::string getName() const = 0;

    [[nodiscard]] virtual bool isReadOnly() const = 0;

    [[nodiscard]] virtual RenderType getDefaultRenderType() const = 0;
};

}  // namespace endstone
```


