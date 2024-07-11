

# File mob.h

[**File List**](files.md) **>** [**actor**](dir_dd7779a583e02d88c9a89a2c881c3946.md) **>** [**mob.h**](mob_8h.md)

[Go to the documentation of this file](mob_8h.md)


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

#include "endstone/actor/actor.h"

namespace endstone {
class Mob : public Actor {
public:
    [[nodiscard]] virtual bool isGliding() const = 0;
};
}  // namespace endstone
```


