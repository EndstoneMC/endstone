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

#include <string_view>

#include "bedrock/bedrock.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"
#include "bedrock/world/item/item_stack_base.h"

class ItemStack : public ItemStackBase {
public:
    static const ItemStack EMPTY_ITEM;
    ItemStack() = default;
    explicit ItemStack(const BlockType &block, int count = 1);
    explicit ItemStack(const ItemInstance &);
    explicit ItemStack(std::string_view name, int count = 1, int aux_value = 0, CompoundTag const *user_data = nullptr);
    explicit ItemStack(Item const &item, int count = 1, int aux_value = 0, CompoundTag const *user_data = nullptr);
    ItemStack(const ItemStack &rhs) = default;
    ItemStack &operator=(const ItemStack &rhs) = default;

    void reinit(const BlockType &, int) override;
    void reinit(const Item &, int, int) override;
    void reinit(std::string_view name, int count, int aux_value) override;
    void setNull(std::optional<std::string>) override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::string toDebugString() const override;

    [[nodiscard]] const ItemStackNetIdVariant &getItemStackNetIdVariant() const;

private:
    ItemStackNetIdVariant network_id_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStack, 152, 152);
