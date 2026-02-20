

# File item.h

[**File List**](files.md) **>** [**actor**](dir_dd7779a583e02d88c9a89a2c881c3946.md) **>** [**item.h**](item_8h.md)

[Go to the documentation of this file](item_8h.md)


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
#include <optional>

#include "endstone/actor/actor.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
class Item : public Actor {
public:
    [[nodiscard]] virtual ItemStack getItemStack() const = 0;

    virtual void setItemStack(const ItemStack &stack) = 0;

    [[nodiscard]] virtual int getPickupDelay() const = 0;

    virtual void setPickupDelay(int delay) = 0;

    virtual void setUnlimitedLifetime(bool unlimited) = 0;

    [[nodiscard]] virtual bool isUnlimitedLifetime() const = 0;

    virtual void setThrower(std::optional<std::int64_t> thrower) = 0;

    [[nodiscard]] virtual std::optional<std::int64_t> getThrower() const = 0;
};

}  // namespace endstone
```


