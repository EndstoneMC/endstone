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

namespace endstone {
/**
 * @brief Represents the metadata of a generic item.
 */
class ItemMeta {
public:
    static ItemMeta EMPTY;
    enum class Type {
        Item = 0,
        Map = 1,
        Count = 2,
        None = Item,
    };

    ItemMeta() = default;
    explicit ItemMeta(const ItemMeta *meta)
    {
        if (meta == nullptr) {
            return;
        }
        *this = *meta;
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
        // TODO(item): more checks here
        return !(hasDisplayName() || hasLore() || hasEnchants() || hasDamage());
    }

    /**
     * @brief Creates a clone of the current metadata.
     *
     * @return A copy of the metadata containing the same state as the original.
     */
    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const
    {
        return std::make_unique<ItemMeta>(*this);
    }

    /**
     * @brief Checks for existence of a display name.
     *
     * @return true if this has a display name
     */
    [[nodiscard]] virtual bool hasDisplayName() const
    {
        return display_name_.has_value();
    }

    /**
     * @brief Gets the display name that is set.
     *
     * @return the display name that is set
     */
    [[nodiscard]] virtual std::optional<std::string> getDisplayName() const
    {
        if (!hasDisplayName()) {
            return std::nullopt;
        }
        return display_name_;
    }

    /**
     * @brief Sets the display name.
     *
     * @param name the name to set
     */
    virtual void setDisplayName(std::optional<std::string> name)
    {
        if (!name.has_value() || name.value().empty()) {
            display_name_.reset();
        }
        else {
            display_name_ = std::move(name.value());
        }
    }

    /**
     * @brief Checks for existence of lore.
     *
     * @return true if this has lore
     */
    [[nodiscard]] virtual bool hasLore() const
    {
        return !lore_.empty();
    }

    /**
     * @brief Gets the lore that is set.
     *
     * @return a list of lore that is set
     */
    [[nodiscard]] virtual std::optional<std::vector<std::string>> getLore() const
    {
        if (!hasLore()) {
            return std::nullopt;
        }
        return lore_;
    }

    /**
     * @brief Sets the lore for this item or removes lore when given std::nullopt.
     *
     * @param lore the lore that will be set
     */
    virtual void setLore(std::optional<std::vector<std::string>> lore)
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
    [[nodiscard]] virtual bool hasDamage() const
    {
        return damage_ > 0;
    }

    /**
     * @brief Gets the damage
     *
     * @return the damage
     */
    [[nodiscard]] virtual int getDamage() const
    {
        return damage_;
    }

    /**
     * @brief Sets the damage
     *
     * @param damage item damage
     */
    virtual void setDamage(int damage)
    {
        damage_ = damage;
    }

    /**
     * @brief Checks for the existence of any enchantments.
     *
     * @return true if an enchantment exists on this meta
     */
    [[nodiscard]] virtual bool hasEnchants() const
    {
        return !enchantments_.empty();
    }

    /**
     * @brief Checks for existence of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return true if this enchantment exists for this meta
     */
    [[nodiscard]] virtual bool hasEnchant(const std::string &id) const
    {
        return hasEnchants() && enchantments_.contains(id);
    }

    /**
     * @brief Checks for the level of the specified enchantment.
     *
     * @param id enchantment id to check
     * @return The level that the specified enchantment has, or 0 if none
     */
    [[nodiscard]] virtual int getEnchantLevel(const std::string &id) const
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
    [[nodiscard]] virtual std::unordered_map<std::string, int> getEnchants() const
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
    virtual bool addEnchant(const std::string &id, int level, bool force)
    {
        // TODO: we should do the level limit check here
        const auto old = getEnchantLevel(id);
        enchantments_[id] = level;
        return old == 0 || old != level;
    }

    /**
     * @brief Removes the specified enchantment from this item meta.
     *
     * @param id Enchantment id to remove
     * @return true if the item meta changed as a result of this call, false
     *     otherwise
     */
    virtual bool removeEnchant(const std::string &id)
    {
        return enchantments_.erase(id) > 0;
    }

    /**
     * @brief Removes all enchantments from this item meta.
     */
    virtual void removeEnchants()
    {
        enchantments_.clear();
    }

private:
    std::optional<std::string> display_name_;
    std::vector<std::string> lore_;
    std::unordered_map<std::string, int> enchantments_;
    int damage_ = 0;
};

inline ItemMeta ItemMeta::EMPTY;
}  // namespace endstone
