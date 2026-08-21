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
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class Block;

/**
 * Called when a player enchants an item at an enchanting table.
 *
 * @note Cancelling the event leaves the item, the player's experience levels and the lapis lazuli untouched.
 */
class EnchantItemEvent final : public Cancellable<PlayerEvent> {
public:
    using Enchantments = std::unordered_map<const Enchantment *, int>;

    ENDSTONE_EVENT(EnchantItemEvent);

    EnchantItemEvent(const NotNull<Player> &player, const NotNull<Block> &enchant_block, ItemStack item,
                     int exp_level_cost, Enchantments enchants_to_add, const Enchantment *enchantment_hint,
                     int level_hint, int which_button)
        : Cancellable(player), enchant_block_(enchant_block), item_(std::move(item)), exp_level_cost_(exp_level_cost),
          enchants_to_add_(std::move(enchants_to_add)), enchantment_hint_(enchantment_hint), level_hint_(level_hint),
          which_button_(which_button)
    {
    }

    /**
     * Gets the enchanting table involved in this event.
     *
     * @return the enchanting table
     */
    [[nodiscard]] const NotNull<Block> &getEnchantBlock() const { return enchant_block_; }

    /**
     * Gets the item being enchanted.
     *
     * @return the item being enchanted
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    /**
     * Sets the item to enchant.
     *
     * @param item the item to enchant
     */
    void setItem(ItemStack item) { item_ = std::move(item); }

    /**
     * Gets the minimum player level required by the selected option.
     *
     * The number of levels consumed remains determined by the selected button.
     *
     * @return the minimum required player level
     */
    [[nodiscard]] int getExpLevelCost() const { return exp_level_cost_; }

    /**
     * Sets the minimum player level required by the selected option.
     *
     * @param exp_level_cost the minimum required player level
     */
    void setExpLevelCost(int exp_level_cost) { exp_level_cost_ = exp_level_cost; }

    /**
     * Gets the enchantments that will be applied to the item.
     *
     * @return the enchantments and their levels
     */
    [[nodiscard]] const Enchantments &getEnchantsToAdd() const { return enchants_to_add_; }

    /**
     * Gets the enchantments that will be applied to the item.
     *
     * @return the enchantments and their levels
     */
    [[nodiscard]] Enchantments &getEnchantsToAdd() { return enchants_to_add_; }

    /**
     * Sets the enchantments to apply to the item.
     *
     * @param enchants_to_add the enchantments and their levels
     */
    void setEnchantsToAdd(Enchantments enchants_to_add) { enchants_to_add_ = std::move(enchants_to_add); }

    /**
     * Gets the enchantment shown as the hint for the selected option.
     *
     * @return the hinted enchantment, or nullptr if no hint is available
     */
    [[nodiscard]] const Enchantment *getEnchantmentHint() const { return enchantment_hint_; }

    /**
     * Gets the level shown for the enchantment hint.
     *
     * @return the hinted enchantment level
     */
    [[nodiscard]] int getLevelHint() const { return level_hint_; }

    /**
     * Gets the selected enchanting button.
     *
     * @return the selected button, from 0 to 2
     */
    [[nodiscard]] int getWhichButton() const { return which_button_; }

private:
    NotNull<Block> enchant_block_;
    ItemStack item_;
    int exp_level_cost_;
    Enchantments enchants_to_add_;
    const Enchantment *enchantment_hint_;
    int level_hint_;
    int which_button_;
};

}  // namespace endstone
