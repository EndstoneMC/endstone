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

#include "bedrock/world/item/item_stack.h"

#include "bedrock/world/item/item_instance.h"

const ItemStack ItemStack::EMPTY_ITEM{};

ItemStack::ItemStack(const BlockType &block, int count) : ItemStackBase(block, count) {}

ItemStack::ItemStack(const ItemInstance &item_instance) : ItemStackBase(item_instance) {}

ItemStack::ItemStack(std::string_view name, int count, int aux_value, CompoundTag const *user_data)
    : ItemStackBase(name, count, aux_value, user_data)
{
}

ItemStack::ItemStack(Item const &item, int count, int aux_value, CompoundTag const *user_data)
    : ItemStackBase(item, count, aux_value, user_data)
{
}

void ItemStack::reinit(const BlockType &block, int count)
{
    const ItemStack rhs(block, count);
    operator=(rhs);
}

void ItemStack::reinit(const Item &item, const int count, const int aux_value)
{
    const ItemStack rhs(item, count, aux_value, nullptr);
    operator=(rhs);
}

void ItemStack::reinit(const std::string_view name, const int count, const int aux_value)
{
    const ItemStack rhs(name, count, aux_value, nullptr);
    operator=(rhs);
}

void ItemStack::setNull(std::optional<std::string> basic_string)
{
    ItemStackBase::setNull(basic_string);
    network_id_ = ItemStackNetIdVariant();
}

std::string ItemStack::toString() const
{
    return ItemStackBase::toString() + network_id_.toString();
}

std::string ItemStack::toDebugString() const
{
    return ItemStackBase::toDebugString() + network_id_.toString();
}

const ItemStackNetIdVariant &ItemStack::getItemStackNetIdVariant() const
{
    return network_id_;
}
