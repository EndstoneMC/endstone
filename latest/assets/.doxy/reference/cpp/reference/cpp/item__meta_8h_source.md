

# File item\_meta.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**meta**](dir_2d728641c8c30e7cdff7ab60efc98406.md) **>** [**item\_meta.h**](item__meta_8h.md)

[Go to the documentation of this file](item__meta_8h.md)


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

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "endstone/enchantments/enchantment.h"

namespace endstone {
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

    [[nodiscard]] virtual Type getType() const
    {
        return Type::Item;
    }

    [[nodiscard]] virtual bool isEmpty() const
    {
        return !(hasDisplayName() || hasLore() || hasEnchants() || hasRepairCost() || isUnbreakable() || hasDamage());
    }

    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const
    {
        return std::make_unique<ItemMeta>(this);
    }

    [[nodiscard]] bool hasDisplayName() const
    {
        return !display_name_.empty();
    }

    [[nodiscard]] std::string getDisplayName() const
    {
        return display_name_;
    }

    void setDisplayName(std::optional<std::string> name)
    {
        display_name_ = std::move(name.value_or(""));
    }

    [[nodiscard]] bool hasLore() const
    {
        return !lore_.empty();
    }

    [[nodiscard]] std::vector<std::string> getLore() const
    {
        return lore_;
    }

    void setLore(std::optional<std::vector<std::string>> lore)
    {
        if (!lore.has_value() || lore.value().empty()) {
            lore_.clear();
        }
        else {
            lore_ = std::move(lore.value());
        }
    }

    [[nodiscard]] bool hasDamage() const
    {
        return damage_ > 0;
    }

    [[nodiscard]] int getDamage() const
    {
        return damage_;
    }

    void setDamage(int damage)
    {
        damage_ = damage;
    }

    [[nodiscard]] bool hasEnchants() const
    {
        return !enchantments_.empty();
    }

    [[nodiscard]] bool hasEnchant(const std::string &id) const
    {
        return hasEnchants() && enchantments_.contains(id);
    }

    [[nodiscard]] int getEnchantLevel(const std::string &id) const
    {
        if (!hasEnchant(id)) {
            return 0;
        }
        return enchantments_.at(id);
    }

    [[nodiscard]] std::unordered_map<std::string, int> getEnchants() const
    {
        if (hasEnchants()) {
            return enchantments_;
        }
        return {};
    }

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

    bool removeEnchant(const std::string &id)
    {
        return enchantments_.erase(id) > 0;
    }

    void removeEnchants()
    {
        enchantments_.clear();
    }

    [[nodiscard]] bool hasRepairCost() const
    {
        return repair_cost_ > 0;
    }

    [[nodiscard]] int getRepairCost() const
    {
        return repair_cost_;
    }

    void setRepairCost(int cost)
    {
        repair_cost_ = cost;
    }

    [[nodiscard]] bool isUnbreakable() const
    {
        return unbreakable_;
    }

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
```


