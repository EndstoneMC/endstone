

# File player\_inventory.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**player\_inventory.h**](player__inventory_8h.md)

[Go to the documentation of this file](player__inventory_8h.md)


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

#include "endstone/inventory/inventory.h"

namespace endstone {

class PlayerInventory : public Inventory {
public:
    [[nodiscard]] virtual std::optional<ItemStack> getHelmet() const = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getChestplate() const = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getLeggings() const = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getBoots() const = 0;

    virtual void setHelmet(std::optional<ItemStack> helmet) = 0;

    virtual void setChestplate(std::optional<ItemStack> chestplate) = 0;

    virtual void setLeggings(std::optional<ItemStack> leggings) = 0;

    virtual void setBoots(std::optional<ItemStack> boots) = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getItemInMainHand() const = 0;

    virtual void setItemInMainHand(std::optional<ItemStack> item) = 0;

    [[nodiscard]] virtual std::optional<ItemStack> getItemInOffHand() const = 0;

    virtual void setItemInOffHand(std::optional<ItemStack> item) = 0;

    [[nodiscard]] virtual int getHeldItemSlot() const = 0;

    virtual void setHeldItemSlot(int slot) = 0;
};

}  // namespace endstone
```


