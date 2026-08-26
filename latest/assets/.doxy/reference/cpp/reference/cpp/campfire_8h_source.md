

# File campfire.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**campfire.h**](campfire_8h.md)

[Go to the documentation of this file](campfire_8h.md)


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

#include <optional>

#include "endstone/block/block_actor_state.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class Campfire : public BlockActorState {
public:
    [[nodiscard]] virtual int getSize() const = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getItem(int index) const = 0;

    virtual void setItem(int index, const std::optional<ItemStack> &item) = 0;

    [[nodiscard]] virtual int getCookTime(int index) const = 0;

    virtual void setCookTime(int index, int cook_time) = 0;
};

}  // namespace endstone
```


