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

/**
 * Called when an enchanting table prepares offers for an item.
 *
 * Set an offer to std::nullopt to hide it, or assign a new EnchantmentOffer to replace it. Each offer contains every
 * enchantment that Bedrock applies when its corresponding button is selected.
 *
 * @note This event can be called multiple times while the enchanting table is open.
 * @note Cancelling the event removes all enchanting offers.
 */
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

    /**
     * Gets the player for whom the offers are being prepared.
     *
     * @return the player viewing the enchanting table
     */
    [[nodiscard]] const NotNull<Player> &getEnchanter() const { return enchanter_; }

    /**
     * Gets the enchanting table involved in this event.
     *
     * @return the enchanting table
     */
    [[nodiscard]] const NotNull<Block> &getEnchantBlock() const { return enchant_block_; }

    /**
     * Gets the item for which offers are being prepared.
     *
     * @return the item being enchanted
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    /**
     * Gets the three enchanting offers. A missing value indicates that the corresponding slot has no offer.
     *
     * @return the enchanting offers
     */
    [[nodiscard]] const Offers &getOffers() const { return offers_; }

    /**
     * Gets the three enchanting offers. A missing value indicates that the corresponding slot has no offer.
     *
     * @return the enchanting offers
     */
    [[nodiscard]] Offers &getOffers() { return offers_; }

    /**
     * Sets the three enchanting offers.
     *
     * @param offers the enchanting offers
     */
    void setOffers(Offers offers) { offers_ = std::move(offers); }

    /**
     * Gets the enchanting bonus provided by nearby bookshelves.
     *
     * @return the number of contributing bookshelves
     */
    [[nodiscard]] int getEnchantmentBonus() const { return enchantment_bonus_; }

private:
    NotNull<Player> enchanter_;
    NotNull<Block> enchant_block_;
    ItemStack item_;
    Offers offers_;
    int enchantment_bonus_;
};

}  // namespace endstone
