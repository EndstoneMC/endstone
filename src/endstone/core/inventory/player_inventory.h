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
    void addItem(const ItemStack &item) override;
    [[nodiscard]] std::vector<std::unique_ptr<ItemStack>> getContents() const override;
    [[nodiscard]] int first(const ItemStack &item) const override;
    [[nodiscard]] bool isEmpty() const override;
    void clear() override;

    [[nodiscard]] std::unique_ptr<ItemStack> getHelmet() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getChestplate() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getLeggings() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getBoots() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItemInMainHand() const override;
    [[nodiscard]] std::unique_ptr<ItemStack> getItemInOffHand() const override;
    [[nodiscard]] int getHeldItemSlot() const override;
    void setHeldItemSlot(int slot) override;

private:
    ::Player &holder_;
};

}  // namespace endstone::core
