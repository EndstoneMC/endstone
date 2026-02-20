

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
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "endstone/enchantments/enchantment.h"

#define ENDSTONE_ITEM_META_TYPE(type) static constexpr auto MetaType = Type::type;

namespace endstone {
namespace core {
class ItemMetaExtras;
}
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

    [[nodiscard]] virtual Type getType() const = 0;

    [[nodiscard]] virtual bool hasDisplayName() const = 0;

    [[nodiscard]] virtual std::string getDisplayName() const = 0;

    virtual void setDisplayName(std::optional<std::string> name) = 0;

    [[nodiscard]] virtual bool hasLore() const = 0;

    [[nodiscard]] virtual std::vector<std::string> getLore() const = 0;

    virtual void setLore(std::optional<std::vector<std::string>> lore) = 0;

    [[nodiscard]] virtual bool hasEnchants() const = 0;

    [[nodiscard]] virtual bool hasEnchant(EnchantmentId id) const = 0;

    [[nodiscard]] virtual int getEnchantLevel(EnchantmentId id) const = 0;

    [[nodiscard]] virtual std::unordered_map<const Enchantment *, int> getEnchants() const = 0;

    [[nodiscard]] virtual bool addEnchant(EnchantmentId id, int level, bool force) = 0;

    virtual bool removeEnchant(EnchantmentId id) = 0;

    virtual void removeEnchants() = 0;

    [[nodiscard]] virtual bool hasConflictingEnchant(EnchantmentId id) const = 0;

    [[nodiscard]] virtual bool isUnbreakable() const = 0;

    virtual void setUnbreakable(bool unbreakable) = 0;

    [[nodiscard]] virtual bool hasDamage() const = 0;

    [[nodiscard]] virtual int getDamage() const = 0;

    virtual void setDamage(int damage) = 0;
    [[nodiscard]] virtual bool hasRepairCost() const = 0;

    [[nodiscard]] virtual int getRepairCost() const = 0;

    virtual void setRepairCost(int cost) = 0;

    [[nodiscard]] virtual std::unique_ptr<ItemMeta> clone() const = 0;

    [[nodiscard]] virtual const core::ItemMetaExtras &getExtras() const = 0;

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
```


