

# File enchant\_item\_event.h

[**File List**](files.md) **>** [**enchantment**](dir_93880425cc5b6b6884a20fefe96c90bf.md) **>** [**enchant\_item\_event.h**](enchant__item__event_8h.md)

[Go to the documentation of this file](enchant__item__event_8h.md)


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

#include <unordered_map>
#include <utility>

#include "endstone/enchantments/enchantment.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/inventory/inventory_event.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/player.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Block;

class EnchantItemEvent final : public Cancellable<InventoryEvent> {
public:
    using Enchantments = std::unordered_map<const Enchantment *, int>;

    ENDSTONE_EVENT(EnchantItemEvent);

    EnchantItemEvent(Inventory &inventory, const NotNull<Player> &enchanter, const NotNull<Block> &enchant_block,
                     ItemStack item, int exp_level_cost, Enchantments enchants_to_add, int which_button)
        : Cancellable(inventory), enchanter_(enchanter), enchant_block_(enchant_block), item_(std::move(item)),
          exp_level_cost_(exp_level_cost), enchants_to_add_(std::move(enchants_to_add)), which_button_(which_button)
    {
    }

    [[nodiscard]] const NotNull<Player> &getEnchanter() const { return enchanter_; }

    [[nodiscard]] const NotNull<Block> &getEnchantBlock() const { return enchant_block_; }

    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    void setItem(ItemStack item) { item_ = std::move(item); }

    [[nodiscard]] int getExpLevelCost() const { return exp_level_cost_; }

    void setExpLevelCost(int exp_level_cost) { exp_level_cost_ = exp_level_cost; }

    [[nodiscard]] const Enchantments &getEnchantsToAdd() const { return enchants_to_add_; }

    [[nodiscard]] Enchantments &getEnchantsToAdd() { return enchants_to_add_; }

    [[nodiscard]] int whichButton() const { return which_button_; }

private:
    NotNull<Player> enchanter_;
    NotNull<Block> enchant_block_;
    ItemStack item_;
    int exp_level_cost_;
    Enchantments enchants_to_add_;
    int which_button_;
};

}  // namespace endstone
```


