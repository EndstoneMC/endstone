

# File enchantment.h

[**File List**](files.md) **>** [**enchantments**](dir_5154bf3e53eefb0e1f350f5612107967.md) **>** [**enchantment.h**](enchantment_8h.md)

[Go to the documentation of this file](enchantment_8h.md)


```C++
// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/namespaced_key.h"

namespace endstone {
class ItemStack;

class Enchantment {
public:
    virtual ~Enchantment() = default;

    [[nodiscard]] virtual NamespacedKey getKey() const = 0;

    [[nodiscard]] virtual int getMaxLevel() const = 0;

    [[nodiscard]] virtual int getStartLevel() const = 0;

    [[nodiscard]] virtual bool conflictsWith(const Enchantment &other) const = 0;

    [[nodiscard]] virtual bool canEnchantItem(const ItemStack &item) const = 0;
};
}  // namespace endstone
```


