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

#include "bedrock/world/item/crafting/recipe_ingredient.h"

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/item_instance.h"

RecipeIngredient RecipeIngredient::EMPTY_INGREDIENT{};

RecipeIngredient::RecipeIngredient() = default;

RecipeIngredient::RecipeIngredient(const ItemDescriptor &descriptor, uint16_t stack_size)
    : ItemDescriptorCount(descriptor, stack_size)
{
}

RecipeIngredient::RecipeIngredient(const Block &block, uint16_t stack_size)
    : ItemDescriptorCount(ItemDescriptor(block), stack_size)
{
}

RecipeIngredient::RecipeIngredient(const BlockType &block, uint16_t stack_size)
    : ItemDescriptorCount(ItemDescriptor(block), stack_size)
{
}

RecipeIngredient::RecipeIngredient(const Item &item, int aux_value, uint16_t stack_size)
    : ItemDescriptorCount(ItemDescriptor(item, aux_value), stack_size)
{
}

RecipeIngredient::RecipeIngredient(const ItemTag &item_tag, uint16_t stack_size)
    : ItemDescriptorCount(ItemDescriptor(item_tag), stack_size)
{
}

RecipeIngredient::RecipeIngredient(std::string_view item, int aux_value, uint16_t stack_size)
    : ItemDescriptorCount(ItemDescriptor(item, aux_value), stack_size)
{
}

RecipeIngredient::RecipeIngredient(ReadOnlyBinaryStream & /*stream*/) {}

bool RecipeIngredient::isEmpty() const
{
    return getStackSize() == 0 && isNull();
}

ItemInstance toItemInstance(const RecipeIngredient &ingredient)
{
    const auto *item = ingredient.getItem();
    if (item == nullptr) {
        return ItemInstance();
    }
    return ItemInstance(ItemStack(*item, ingredient.getStackSize(), ingredient.getAuxValue()));
}
