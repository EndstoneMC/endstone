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

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "endstone/enchantments/enchantment.h"

namespace endstone {
/**
 * @brief Represents the metadata of a generic item.
 */
class ItemMeta {
public:
    enum class Type {
        Item,
        Map,
    };

    explicit ItemMeta(const ItemMeta *meta)
    {
        if (meta == nullptr) {
            return;
        }
        display_name_ = meta->display_name_;
        lore_ = meta->lore_;
        enchantments_ = meta->enchantments_;
        repair_cost_ = meta->repair_cost_;
        damage_ = meta->damage_;
        unbreakable_ = meta->unbreakable_;
    }

    virtual ~ItemMeta() = default;

    /**
     * @brief Gets the type of this item meta
     *
     * @return type of this item meta
     */
    [[nodiscard]] virtual Type getType() const
    {
        return Type::Item;
    }

    /**
     * @brief Checks if the item metadata is empty.
     *
     * @return true if the metadata is empty, false otherwise.
     */
    [[nodiscard]] virtual bool isEmpty() const
    {
        return !(hasDisplayName() || hasLore() || hasEnchants() || hasRepairCost() || isUnbreakable() || hasDamage());
    }

    /**
     * @brief Creates a clone of the current metadata.
     * @return A copy of the metadata containing the same state as the original.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const
    {
        return std::make_unique<ItemMeta>(this);
    }

    /**
     * @brief Checks for existence of a display name.
     *
     * @return true if this has a display name
     */
    [[nodiscard]] bool hasDisplayName() const
    {
        return !display_name_.empty();
    }

    /**
     * @brief Gets the display name that is set.
     *
     * @return the display name that is set
     */
    [[nodiscard]] std::string getDisplayName() const
    {
        return display_name_;
    }

    /**
     * @brief Sets the display name.
     *
     * @param name the name to set
     */
    void setDisplayName(std::optional<std::string> name)
    {
        display_name_ = std::move(name.value_or(""));
    }

    /**
     * @brief Checks for existence of lore.
     *
     * @return true if this has lore
     */
    [[nodiscard]] bool hasLore() const
    {
        return !lore_.empty();
    }

    /**
     * @brief Gets the lore that is set.
     *
     * @return a list of lore that is set
     */
    [[nodiscard]] std::vector<std::string> getLore() const
    {
        return lore_;
    }

    /**
     * @brief Sets the lore for this item or removes lore when given std::nullopt.
     *
     * @param lore the lore that will be set
     */
    void setLore(std::optional<std::vector<std::string>> lore)
    {
        if (!lore.has_value() || lore.value().empty()) {
            lore_.clear();
        }
        else {
            lore_ = std::move(lore.value());
        }
    }

    /**
     * @brief Checks to see if this item has damage
     *
     * @return true if this has damage
     */
    [[nodiscard]] bool hasDamage() const
    {
        return damage_ > 0;
    }

    /**
     * @brief Gets the damage
     *
     * @return the damage
     */
    [[nodiscard]] int getDamage() const
    {
        return damage_;
    }

    /**
     * @brief Sets the damage
     *
     * @param damage item damage
     */
    void setDamage(int damage)
    {
        damage_ = damage;
    }

    /**
     * @brief Checks for the existence of any enchantments.
     *
     * @return true if an enchantment exists on this meta
     */
    [[nodiscard]] bool hasEnchants() const
    {
        return !enchantments_.empty();
    }

    /**
     * @brief Checks for existence of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return true if this enchantment exists for this meta
     */
    [[nodiscard]] bool hasEnchant(const std::string &id) const
    {
        return hasEnchants() && enchantments_.contains(id);
    }

    /**
     * @brief Checks for the level of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return The level that the specified enchantment has, or 0 if none
     */
    [[nodiscard]] int getEnchantLevel(const std::string &id) const
    {
        if (!hasEnchant(id)) {
            return 0;
        }
        return enchantments_.at(id);
    }

    /**
     * @brief Returns a copy the enchantments in this ItemMeta.
     *
     * Returns an empty map if none.
     *
     * @return An immutable copy of the enchantments
     */
    [[nodiscard]] std::unordered_map<std::string, int> getEnchants() const
    {
        if (hasEnchants()) {
            return enchantments_;
        }
        return {};
    }

    /**
     * @brief Adds the specified enchantment to this item meta.
     *
     * @param id Enchantment id to add
     * @param level Level for the enchantment
     * @param force this indicates the enchantment should be applied, ignoring the level limit
     * @return true if the item meta changed as a result of this call, false otherwise
     */
    bool addEnchant(const std::string &id, int level, bool force = false)
    {
        const auto *ench = Enchantment::get(id);
        if (!ench) {
            return false;
        }
        if (force || level >= ench->getStartLevel() && level <= ench->getMaxLevel()) {
            const auto old = getEnchantLevel(id);
            enchantments_[id] = level;
            return old == 0 || old != level;
        }
        return false;
    }

    /**
     * @brief Removes the specified enchantment from this item meta.
     *
     * @param id Enchantment id to remove
     * @return true if the item meta changed as a result of this call, false
     *     otherwise
     */
    bool removeEnchant(const std::string &id)
    {
        return enchantments_.erase(id) > 0;
    }

    /**
     * @brief Removes all enchantments from this item meta.
     */
    void removeEnchants()
    {
        enchantments_.clear();
    }

    /**
     * @brief Checks to see if this has a repair penalty
     *
     * @return true if this has a repair penalty
     */
    [[nodiscard]] bool hasRepairCost() const
    {
        return repair_cost_ > 0;
    }

    /**
     * @brief Gets the repair penalty
     *
     * @return the repair penalty
     */
    [[nodiscard]] int getRepairCost() const
    {
        return repair_cost_;
    }

    /**
     * @brief Sets the repair penalty
     *
     * @param cost repair penalty
     */
    void setRepairCost(int cost)
    {
        repair_cost_ = cost;
    }

    /**
     * @brief Return if the unbreakable tag is true. An unbreakable item will not lose durability.
     *
     * @return true if the unbreakable tag is true
     */
    [[nodiscard]] bool isUnbreakable() const
    {
        return unbreakable_;
    }

    /**
     * @brief Sets the unbreakable tag. An unbreakable item will not lose durability.
     *
     * @param unbreakable true if set unbreakable
     */
    void setUnbreakable(bool unbreakable)
    {
        unbreakable_ = unbreakable;
    }

    template <typename T>
    T *as()
    {
        if (getType() == T::TYPE) {
            return static_cast<T *>(this);
        }
        return nullptr;
    }

    template <typename T>
    const T *as() const
    {
        if (getType() == T::TYPE) {
            return static_cast<const T *>(this);
        }
        return nullptr;
    }

private:
    std::string display_name_;
    std::vector<std::string> lore_;
    std::unordered_map<std::string, int> enchantments_;
    int repair_cost_ = 0;
    int damage_ = 0;
    bool unbreakable_ = false;
};
}  // namespace endstone
