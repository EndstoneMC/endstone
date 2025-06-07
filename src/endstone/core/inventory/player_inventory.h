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
    explicit EndstonePlayerInventory(::Player &holder) : EndstoneInventory(holder.getInventory()), holder_(holder){};

    [[nodiscard]] int getSize() const override;
    [[nodiscard]] int getMaxStackSize() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItem(int index) const override;
    void setItem(int index, const ItemStack *item) override;
    std::unordered_map<int, const ItemStack *> addItem(std::vector<ItemStack const *> items) override;
    std::unordered_map<int, const ItemStack *> removeItem(std::vector<ItemStack const *> items) override;
    [[nodiscard]] std::vector<std::unique_ptr<ItemStack>> getContents() const override;
    Result<void> setContents(std::vector<ItemStack const *> items) override;
    [[nodiscard]] Result<bool> contains(const std::string &type) const override;
    [[nodiscard]] bool contains(const ItemStack &item) const override;
    [[nodiscard]] bool contains(const ItemStack &item, int amount) const override;
    [[nodiscard]] Result<bool> containsAtLeast(const std::string &type, int amount) const override;
    [[nodiscard]] bool containsAtLeast(const ItemStack &item, int amount) const override;
    [[nodiscard]] Result<std::unordered_map<int, std::unique_ptr<ItemStack>>> all(
        const std::string &type) const override;
    [[nodiscard]] std::unordered_map<int, std::unique_ptr<ItemStack>> all(const ItemStack &item) const override;
    [[nodiscard]] Result<int> first(const std::string &type) const override;
    [[nodiscard]] int first(const ItemStack &item) const override;
    [[nodiscard]] int firstEmpty() const override;
    [[nodiscard]] bool isEmpty() const override;
    Result<void> remove(const std::string &type) override;
    void remove(const ItemStack &item) override;
    void clear(int index) override;
    void clear() override;

    [[nodiscard]] std::unique_ptr<ItemStack> getHelmet() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getChestplate() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getLeggings() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getBoots() const override;
    void setHelmet(const ItemStack *helmet) override;
    void setChestplate(const ItemStack *chestplate) override;
    void setLeggings(const ItemStack *leggings) override;
    void setBoots(const ItemStack *boots) override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItemInMainHand() const override;
    void setItemInMainHand(const ItemStack *item) override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItemInOffHand() const override;
    void setItemInOffHand(const ItemStack *item) override;
    [[nodiscard]] int getHeldItemSlot() const override;
    Result<void> setHeldItemSlot(int slot) override;

private:
    ::Player &holder_;
};

}  // namespace endstone::core
