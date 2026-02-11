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

#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/inventory/item_stack.h"

namespace endstone::core {

class EndstoneItemStack : public ItemStack::Impl {
public:
    explicit EndstoneItemStack(const ::ItemStackBase &item);
    EndstoneItemStack(const EndstoneItemStack &other);
    EndstoneItemStack &operator=(const EndstoneItemStack &other);

    [[nodiscard]] std::unique_ptr<Impl> clone() const override;
    [[nodiscard]] const ItemType &getType() const override;
    void setType(ItemTypeId type) override;
    [[nodiscard]] int getAmount() const override;
    void setAmount(int amount) override;
    [[nodiscard]] int getData() const override;
    void setData(int data) override;
    [[nodiscard]] std::string getTranslationKey() const override;
    [[nodiscard]] int getMaxStackSize() const override;
    [[nodiscard]] bool isSimilar(const Impl &other) const override;
    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta() const override;
    [[nodiscard]] bool hasItemMeta() const override;
    bool setItemMeta(const ItemMeta *meta) override;

    static ::ItemStack toMinecraft(const ItemStack &item);
    static ItemStack fromMinecraft(const ::ItemStackBase &item);
    static const ItemType &getType(const ItemStackBase *item);
    static std::unique_ptr<ItemMeta> getItemMeta(const ItemStackBase *item);
    static bool hasItemMeta(const ItemStackBase *item);
    static bool setItemMeta(ItemStackBase *item, const ItemMeta *meta);

private:
    ItemInstance item_;
};

}  // namespace endstone::core
