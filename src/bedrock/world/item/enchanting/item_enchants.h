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
#include <utility>
#include <vector>

#include "bedrock/world/item/enchanting/enchantment_instance.h"

class ItemEnchants {
public:
    using Enchantments = std::array<std::vector<EnchantmentInstance>, Enchant::_num_activations>;

    [[nodiscard]] int getSlot() const { return slot_; }
    void setSlot(int slot) { slot_ = slot; }
    void setEnchantInstances(Enchantments enchantments) { item_enchants_ = std::move(enchantments); }

    [[nodiscard]] std::vector<EnchantmentInstance> getAllEnchants() const
    {
        std::vector<EnchantmentInstance> result;
        for (const auto &enchants : item_enchants_) {
            result.insert(result.end(), enchants.begin(), enchants.end());
        }
        return result;
    }

private:
    int slot_;
    Enchantments item_enchants_;
};
static_assert(sizeof(ItemEnchants) == 80);
