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

#include "bedrock/nbt/compound_tag.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"

namespace endstone::core {

class EndstoneItemMeta : public ItemMeta {
public:
    EndstoneItemMeta(const EndstoneItemMeta *meta);
    EndstoneItemMeta(const ::CompoundTag &tag);
    [[nodiscard]] Type getType() const override;
    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override;
    [[nodiscard]] bool hasDisplayName() const override;
    [[nodiscard]] std::optional<std::string> getDisplayName() const override;
    void setDisplayName(std::optional<std::string> name) override;
    [[nodiscard]] bool hasLore() const override;
    [[nodiscard]] std::optional<std::vector<std::string>> getLore() const override;
    void setLore(std::optional<std::vector<std::string>> lore) override;
    [[nodiscard]] bool hasDamage() const override;
    [[nodiscard]] int getDamage() const override;
    void setDamage(int damage) override;
    [[nodiscard]] bool hasEnchants() const override;
    [[nodiscard]] bool hasEnchant(const std::string &id) const override;
    [[nodiscard]] int getEnchantLevel(const std::string &id) const override;
    [[nodiscard]] std::unordered_map<std::string, int> getEnchants() const override;
    bool addEnchant(const std::string &id, int level, bool force) override;
    bool removeEnchant(const std::string &id) override;
    void removeEnchants() override;
    [[nodiscard]] bool hasRepairCost() const override;
    [[nodiscard]] int getRepairCost() const override;
    void setRepairCost(int cost) override;
    [[nodiscard]] bool isUnbreakable() const override;
    void setUnbreakable(bool unbreakable) override;

    [[nodiscard]] virtual bool applicableTo(const std::string &type) const;
    [[nodiscard]] virtual bool equalsCommon(const EndstoneItemMeta &other) const;
    [[nodiscard]] virtual bool notUncommon() const;
    virtual void applyToItem(::CompoundTag &tag) const;

private:
    std::optional<std::string> display_name_;
    std::vector<std::string> lore_;
    std::unordered_map<std::string, int> enchantments_;
    int repair_cost_ = 0;
    int damage_ = 0;
    bool unbreakable_ = false;
};

}  // namespace endstone::core
