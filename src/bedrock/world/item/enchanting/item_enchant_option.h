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

#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/enchanting/item_enchants.h"

struct ItemEnchantOption {
    [[nodiscard]] int getCost() const { return cost_; }
    void setCost(int cost) { cost_ = cost; }
    [[nodiscard]] const ItemEnchants &getEnchants() const { return enchants_; }
    [[nodiscard]] ItemEnchants &getEnchants() { return enchants_; }
    [[nodiscard]] const std::string &getEnchantName() const { return enchant_name_; }
    [[nodiscard]] const RecipeNetId &getEnchantNetId() const { return enchant_net_id_; }

private:
    int cost_;
    ItemEnchants enchants_;
    std::string enchant_name_;
    RecipeNetId enchant_net_id_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemEnchantOption, 128, 120);
