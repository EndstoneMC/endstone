

# File enchantment\_offer.h

[**File List**](files.md) **>** [**enchantments**](dir_5154bf3e53eefb0e1f350f5612107967.md) **>** [**enchantment\_offer.h**](enchantment__offer_8h.md)

[Go to the documentation of this file](enchantment__offer_8h.md)


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

#include <unordered_map>
#include <utility>

#include "endstone/check.h"
#include "endstone/enchantments/enchantment.h"

namespace endstone {

class EnchantmentOffer {
public:
    using Enchantments = std::unordered_map<const Enchantment *, int>;

    EnchantmentOffer(Enchantments enchants, int cost) : enchants_(std::move(enchants)), cost_(cost)
    {
        validateEnchants(enchants_);
        Preconditions::checkArgument(cost > 0 && cost <= 255, "cost must be between 1 and 255");
    }

    [[nodiscard]] const Enchantments &getEnchants() const { return enchants_; }

    void setEnchants(Enchantments enchants)
    {
        validateEnchants(enchants);
        enchants_ = std::move(enchants);
    }

    [[nodiscard]] int getCost() const { return cost_; }

    void setCost(int cost)
    {
        Preconditions::checkArgument(cost > 0 && cost <= 255, "cost must be between 1 and 255");
        cost_ = cost;
    }

private:
    static void validateEnchants(const Enchantments &enchants)
    {
        Preconditions::checkArgument(!enchants.empty(), "enchants must not be empty");
        for (const auto &[enchantment, level] : enchants) {
            Preconditions::checkArgument(enchantment != nullptr, "enchantment must not be null");
            Preconditions::checkArgument(level > 0, "enchantment level must be greater than 0");
        }
    }

    Enchantments enchants_;
    int cost_;
};

}  // namespace endstone
```


