

# File prepare\_item\_enchant\_event.h

[**File List**](files.md) **>** [**enchantment**](dir_93880425cc5b6b6884a20fefe96c90bf.md) **>** [**prepare\_item\_enchant\_event.h**](prepare__item__enchant__event_8h.md)

[Go to the documentation of this file](prepare__item__enchant__event_8h.md)


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

#include <array>
#include <optional>
#include <utility>

#include "endstone/enchantments/enchantment_offer.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/inventory/inventory_event.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/player.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Block;

class PrepareItemEnchantEvent final : public Cancellable<InventoryEvent> {
public:
    using Offers = std::array<std::optional<EnchantmentOffer>, 3>;

    ENDSTONE_EVENT(PrepareItemEnchantEvent);

    PrepareItemEnchantEvent(Inventory &inventory, const NotNull<Player> &enchanter, const NotNull<Block> &enchant_block,
                            ItemStack item, Offers offers, int enchantment_bonus)
        : Cancellable(inventory), enchanter_(enchanter), enchant_block_(enchant_block), item_(std::move(item)),
          offers_(std::move(offers)), enchantment_bonus_(enchantment_bonus)
    {
    }

    [[nodiscard]] const NotNull<Player> &getEnchanter() const { return enchanter_; }

    [[nodiscard]] const NotNull<Block> &getEnchantBlock() const { return enchant_block_; }

    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    [[nodiscard]] const Offers &getOffers() const { return offers_; }

    [[nodiscard]] Offers &getOffers() { return offers_; }

    void setOffers(Offers offers) { offers_ = std::move(offers); }

    [[nodiscard]] int getEnchantmentBonus() const { return enchantment_bonus_; }

private:
    NotNull<Player> enchanter_;
    NotNull<Block> enchant_block_;
    ItemStack item_;
    Offers offers_;
    int enchantment_bonus_;
};

}  // namespace endstone
```


