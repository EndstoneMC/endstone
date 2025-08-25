// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "bedrock/world/item/enchanting/enchant.h"
#include "endstone/enchantments/enchantment.h"

namespace endstone::core {
class EndstoneEnchantment : public Enchantment {
public:
    EndstoneEnchantment(std::string id, const Enchant &handle);
    [[nodiscard]] std::string getId() const override;
    [[nodiscard]] std::string getTranslationKey() const override;
    [[nodiscard]] int getMaxLevel() const override;
    [[nodiscard]] int getStartLevel() const override;
    [[nodiscard]] bool conflictsWith(const Enchantment &other) const override;
    [[nodiscard]] bool canEnchantItem(const ItemStack &item) const override;

    [[nodiscard]] const Enchant &getHandle() const;

private:
    std::string id_;
    const Enchant &handle_;
};
}  // namespace endstone::core
