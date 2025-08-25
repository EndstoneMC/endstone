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

#include "bedrock/world/item/item_stack.h"
#include "endstone/inventory/item_stack.h"

namespace endstone::core {

class EndstoneItemStack : public ItemStack {
public:
    explicit EndstoneItemStack(const ::ItemStack &item);
    EndstoneItemStack(const EndstoneItemStack &item);

protected:
    [[nodiscard]] bool isEndstoneItemStack() const override;

public:
    [[nodiscard]] std::string getType() const override;
    void setType(const std::string &type) override;
    [[nodiscard]] int getAmount() const override;
    void setAmount(int amount) override;
    [[nodiscard]] int getData() const override;
    void setData(int data) override;
    [[nodiscard]] int getMaxStackSize() const override;
    [[nodiscard]] bool isSimilar(const ItemStack &other) const override;
    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta() const override;
    [[nodiscard]] bool hasItemMeta() const override;
    bool setItemMeta(ItemMeta *meta) override;
    [[nodiscard]] std::unique_ptr<ItemStack> clone() const override;

    static ::ItemStack toMinecraft(const ItemStack *item);
    static std::unique_ptr<EndstoneItemStack> fromMinecraft(const ::ItemStack &item);
    static std::string getType(const ::ItemStack *item);
    static std::unique_ptr<ItemMeta> getItemMeta(const ::ItemStack *item);
    static bool hasItemMeta(const ::ItemStack *item);
    static bool setItemMeta(::ItemStack *item, const ItemMeta *meta);

private:
    ::ItemStack &handle_;
};

}  // namespace endstone::core
