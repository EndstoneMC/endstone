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
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "endstone/enchantments/enchantment.h"
#include "endstone/nbt/tag.h"

#define ENDSTONE_ITEM_META_TYPE(type) static constexpr auto MetaType = Type::type;

namespace endstone {
/**
 * @brief Represents the metadata of a generic item.
 */
class ItemMeta {
public:
    enum class Type {
        Item,
        Book,
        CrossBow,
        Map,
        WritableBook,
    };

    ENDSTONE_ITEM_META_TYPE(Item)

    virtual ~ItemMeta() = default;

    /**
     * @brief Gets the type of this item meta
     *
     * @return type of this item meta
     */
    [[nodiscard]] virtual Type getType() const = 0;

    /**
     * @brief Checks for existence of a display name.
     *
     * @return true if this has a display name
     */
    [[nodiscard]] virtual bool hasDisplayName() const = 0;

    /**
     * @brief Gets the display name that is set.
     *
     * @return the display name that is set
     */
    [[nodiscard]] virtual std::string getDisplayName() const = 0;

    /**
     * @brief Sets the display name.
     *
     * @param name the name to set
     */
    virtual void setDisplayName(std::optional<std::string> name) = 0;

    /**
     * @brief Checks for existence of lore.
     *
     * @return true if this has lore
     */
    [[nodiscard]] virtual bool hasLore() const = 0;

    /**
     * @brief Gets the lore that is set.
     *
     * @return a list of lore that is set
     */
    [[nodiscard]] virtual std::vector<std::string> getLore() const = 0;

    /**
     * @brief Sets the lore for this item or removes lore when given <code>std::nullopt</code>.
     *
     * @param lore the lore that will be set
     */
    virtual void setLore(std::optional<std::vector<std::string>> lore) = 0;

    /**
     * @brief Checks for the existence of any enchantments.
     *
     * @return true if an enchantment exists on this meta
     */
    [[nodiscard]] virtual bool hasEnchants() const = 0;

    /**
     * @brief Checks for existence of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return true if this enchantment exists for this meta
     */
    [[nodiscard]] virtual bool hasEnchant(EnchantmentId id) const = 0;

    /**
     * @brief Checks for the level of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return The level that the specified enchantment has, or 0 if none
     */
    [[nodiscard]] virtual int getEnchantLevel(EnchantmentId id) const = 0;

    /**
     * @brief Returns a copy the enchantments in this ItemMeta.
     *
     * Returns an empty map if none.
     *
     * @return An immutable copy of the enchantments
     */
    [[nodiscard]] virtual std::unordered_map<const Enchantment *, int> getEnchants() const = 0;

    /**
     * @brief Adds the specified enchantment to this item meta.
     *
     * @param id Enchantment id to add
     * @param level Level for the enchantment
     * @param force this indicates the enchantment should be applied, ignoring the level limit
     * @return true if the item meta changed as a result of this call, false otherwise
     */
    [[nodiscard]] virtual bool addEnchant(EnchantmentId id, int level, bool force) = 0;

    /**
     * @brief Removes the specified enchantment from this item meta.
     *
     * @param id Enchantment id to remove
     * @return true if the item meta changed as a result of this call, false
     *     otherwise
     */
    virtual bool removeEnchant(EnchantmentId id) = 0;

    /**
     * @brief Removes all enchantments from this item meta.
     */
    virtual void removeEnchants() = 0;

    /**
     * Checks if the specified enchantment conflicts with any enchantments in
     * this ItemMeta.
     *
     * @param id Enchantment id to test
     * @return true if the enchantment conflicts, false otherwise
     */
    [[nodiscard]] virtual bool hasConflictingEnchant(EnchantmentId id) const = 0;

    /**
     * @brief Return if the unbreakable tag is true. An unbreakable item will not lose durability.
     *
     * @return true if the unbreakable tag is true
     */
    [[nodiscard]] virtual bool isUnbreakable() const = 0;

    /**
     * @brief Sets the unbreakable tag. An unbreakable item will not lose durability.
     *
     * @param unbreakable true if set unbreakable
     */
    virtual void setUnbreakable(bool unbreakable) = 0;

    /**
     * @brief Checks to see if this item has damage
     *
     * @return true if this has damage
     */
    [[nodiscard]] virtual bool hasDamage() const = 0;

    /**
     * @brief Gets the damage
     *
     * @return the damage
     */
    [[nodiscard]] virtual int getDamage() const = 0;

    /**
     * @brief Sets the damage
     *
     * @param damage item damage
     */
    virtual void setDamage(int damage) = 0;
    /**
     * @brief Checks to see if this has a repair penalty
     *
     * @return true if this has a repair penalty
     */
    [[nodiscard]] virtual bool hasRepairCost() const = 0;

    /**
     * @brief Gets the repair penalty
     *
     * @return the repair penalty
     */
    [[nodiscard]] virtual int getRepairCost() const = 0;

    /**
     * @brief Sets the repair penalty
     *
     * @param cost repair penalty
     */
    virtual void setRepairCost(int cost) = 0;

    /**
     * @brief Creates a clone of the current metadata.
     * @return A copy of the metadata containing the same state as the original.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const = 0;

    [[nodiscard]] virtual CompoundTag toNbt() const = 0;

    template <typename T>
        requires std::is_base_of_v<ItemMeta, T>
    T *as()
    {
        if (this->getType() == T::MetaType) {
            return static_cast<T *>(this);
        }
        return nullptr;
    }

    template <typename T>
        requires std::is_base_of_v<ItemMeta, T>
    const T *as() const
    {
        if (this->getType() == T::MetaType) {
            return static_cast<const T *>(this);
        }
        return nullptr;
    }
};
}  // namespace endstone
