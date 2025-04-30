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
        return !(hasDisplayName() || hasLore() || hasDamage());
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
        return display_name_.has_value() && !display_name_.value().empty();
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
            display_name_ = std::nullopt;
        }
        else {
            display_name_ = std::move(name);
        }
    }

    /**
     * @brief Checks for existence of lore.
     *
     * @return true if this has lore
     */
    [[nodiscard]] virtual bool hasLore() const
    {
        return lore_.has_value() && !lore_.value().empty();
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
            lore_ = std::nullopt;
        }
        else {
            lore_ = std::move(lore);
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

    virtual bool addEnchant(std::int16_t id, std::int16_t lvl)
    {
        if (enchants_ && enchants_->contains(id) && enchants_->at(id) == lvl) {
            return false;
        }
        if (!enchants_) {
            enchants_ = {};
        }
        (*enchants_)[id] = lvl;
        return true;
    }

    virtual bool removeEnchant(std::int16_t id)
    {
        if (enchants_ && enchants_->contains(id)) {
            enchants_->erase(id);
            if (enchants_->empty()) {
                enchants_ = std::nullopt;
            }
            return true;
        }
        return false;
    }

    virtual bool removeEnchants()
    {
        if (enchants_) {
            enchants_ = std::nullopt;
            return true;
        }
        return false;
    }

    virtual void setEnchants(std::unordered_map<std::int16_t, std::int16_t> enchants)
    {
        if (!enchants.empty()) {
            enchants_ = std::move(enchants);
        }
        else {
            enchants_ = std::nullopt;
        }
    }

    [[nodiscard]] virtual bool hasEnchant(std::int16_t id) const
    {
        return enchants_ && enchants_->contains(id);
    }

    [[nodiscard]] virtual std::unordered_map<std::int16_t, std::int16_t> getEnchants() const
    {
        return enchants_ ? *enchants_ : std::unordered_map<std::int16_t, std::int16_t>{};
    }

    [[nodiscard]] virtual std::int16_t getEnchantLevel(std::int16_t id) const
    {
        return enchants_ && enchants_->contains(id) ? enchants_->at(id) : static_cast<std::int16_t>(0);
    }

    [[nodiscard]] virtual bool hasEnchants() const
    {
        return enchants_.has_value();
    }

private:
    std::optional<std::unordered_map<std::int16_t, std::int16_t>> enchants_;
    std::optional<std::string> display_name_;
    std::optional<std::vector<std::string>> lore_;
    int damage_ = 0;
};

inline ItemMeta ItemMeta::EMPTY;
}  // namespace endstone
