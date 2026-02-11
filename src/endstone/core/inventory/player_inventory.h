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

#include "bedrock/world/actor/player/player.h"
#include "endstone/core/inventory/inventory.h"
#include "endstone/inventory/player_inventory.h"

namespace endstone::core {

class EndstonePlayerInventory : public EndstoneInventory, public PlayerInventory {
public:
    explicit EndstonePlayerInventory(::Player &holder) : EndstoneInventory(holder.getInventory()), holder_(holder) {};

    [[nodiscard]] int getSize() const override;
    [[nodiscard]] int getMaxStackSize() const override;
    [[nodiscard]] std::optional<ItemStack> getItem(int index) const override;
    void setItem(int index, std::optional<ItemStack> item) override;
    std::unordered_map<int, ItemStack> addItem(std::vector<ItemStack> items) override;
    std::unordered_map<int, ItemStack> removeItem(std::vector<ItemStack> items) override;
    [[nodiscard]] std::vector<std::optional<ItemStack>> getContents() const override;
    void setContents(std::vector<std::optional<ItemStack>> items) override;
    [[nodiscard]] bool contains(const std::string &type) const override;
    [[nodiscard]] bool contains(const ItemStack &item) const override;
    [[nodiscard]] bool contains(const ItemStack &item, int amount) const override;
    [[nodiscard]] bool containsAtLeast(const std::string &type, int amount) const override;
    [[nodiscard]] bool containsAtLeast(const ItemStack &item, int amount) const override;
    [[nodiscard]] std::unordered_map<int, ItemStack> all(const std::string &type) const override;
    [[nodiscard]] std::unordered_map<int, ItemStack> all(const ItemStack &item) const override;
    [[nodiscard]] int first(const std::string &type) const override;
    [[nodiscard]] int first(const ItemStack &item) const override;
    [[nodiscard]] int firstEmpty() const override;
    [[nodiscard]] bool isEmpty() const override;
    void remove(const std::string &type) override;
    void remove(const ItemStack &item) override;
    void clear(int index) override;
    void clear() override;

    [[nodiscard]] std::optional<ItemStack> getHelmet() const override;
    [[nodiscard]] std::optional<ItemStack> getChestplate() const override;
    [[nodiscard]] std::optional<ItemStack> getLeggings() const override;
    [[nodiscard]] std::optional<ItemStack> getBoots() const override;
    void setHelmet(std::optional<ItemStack> helmet) override;
    void setChestplate(std::optional<ItemStack> chestplate) override;
    void setLeggings(std::optional<ItemStack> leggings) override;
    void setBoots(std::optional<ItemStack> boots) override;
    [[nodiscard]] std::optional<ItemStack> getItemInMainHand() const override;
    void setItemInMainHand(std::optional<ItemStack> item) override;
    [[nodiscard]] std::optional<ItemStack> getItemInOffHand() const override;
    void setItemInOffHand(std::optional<ItemStack> item) override;
    [[nodiscard]] int getHeldItemSlot() const override;
    void setHeldItemSlot(int slot) override;

private:
    [[nodiscard]] std::optional<ItemStack> getArmor(ArmorSlot slot) const;
    void setArmor(ArmorSlot slot, std::optional<ItemStack> armor);

    ::Player &holder_;
};

}  // namespace endstone::core
