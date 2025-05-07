

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

namespace endstone {
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

    [[nodiscard]] virtual Type getType() const
    {
        return Type::Item;
    }

    [[nodiscard]] virtual bool isEmpty() const
    {
        // TODO(item): more checks here
        return !(hasDisplayName() || hasLore() || hasEnchants() || hasDamage());
    }

    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const
    {
        return std::make_unique<ItemMeta>(*this);
    }

    [[nodiscard]] virtual bool hasDisplayName() const
    {
        return display_name_.has_value();
    }

    [[nodiscard]] virtual std::optional<std::string> getDisplayName() const
    {
        if (!hasDisplayName()) {
            return std::nullopt;
        }
        return display_name_;
    }

    virtual void setDisplayName(std::optional<std::string> name)
    {
        if (!name.has_value() || name.value().empty()) {
            display_name_.reset();
        }
        else {
            display_name_ = std::move(name.value());
        }
    }

    [[nodiscard]] virtual bool hasLore() const
    {
        return !lore_.empty();
    }

    [[nodiscard]] virtual std::optional<std::vector<std::string>> getLore() const
    {
        if (!hasLore()) {
            return std::nullopt;
        }
        return lore_;
    }

    virtual void setLore(std::optional<std::vector<std::string>> lore)
    {
        if (!lore.has_value() || lore.value().empty()) {
            lore_.clear();
        }
        else {
            lore_ = std::move(lore.value());
        }
    }

    [[nodiscard]] virtual bool hasDamage() const
    {
        return damage_ > 0;
    }

    [[nodiscard]] virtual int getDamage() const
    {
        return damage_;
    }

    virtual void setDamage(int damage)
    {
        damage_ = damage;
    }

    [[nodiscard]] virtual bool hasEnchants() const
    {
        return !enchantments_.empty();
    }

    [[nodiscard]] virtual bool hasEnchant(const std::string &id) const
    {
        return hasEnchants() && enchantments_.contains(id);
    }

    [[nodiscard]] virtual int getEnchantLevel(const std::string &id) const
    {
        if (!hasEnchant(id)) {
            return 0;
        }
        return enchantments_.at(id);
    }

    [[nodiscard]] virtual std::unordered_map<std::string, int> getEnchants() const
    {
        if (hasEnchants()) {
            return enchantments_;
        }
        return {};
    }

    virtual bool addEnchant(const std::string &id, int level, bool force)
    {
        // TODO: we should do the level limit check here
        const auto old = getEnchantLevel(id);
        enchantments_[id] = level;
        return old == 0 || old != level;
    }

    virtual bool removeEnchant(const std::string &id)
    {
        return enchantments_.erase(id) > 0;
    }

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
```


