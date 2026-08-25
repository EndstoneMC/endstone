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

#include "endstone/core/inventory/recipe_ingredient.h"

#include "bedrock/world/item/item_descriptor.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "bedrock/world/item/vanilla_item_tag.h"
#include "endstone/inventory/item_type.h"

namespace endstone::core {

bool EndstoneIngredient::test(const endstone::ItemStack &item) const
{
    const auto minecraft = EndstoneItemStack::toMinecraft(item);
    if (recipe_) {
        return recipe_->itemValidForRecipe(ingredient_, minecraft);
    }
    return ingredient_.sameItem(minecraft.getDescriptor(), true);
}

std::optional<endstone::RecipeIngredient> EndstoneIngredient::fromMinecraft(std::shared_ptr<const ::Recipe> recipe,
                                                                            const ::RecipeIngredient &ingredient)
{
    const auto &descriptor = static_cast<const ::ItemDescriptor &>(ingredient);
    switch (descriptor.getType()) {
    case ItemDescriptor::InternalType::Invalid:
        return std::nullopt;
    case ItemDescriptor::InternalType::ItemTag:
        return ItemTagIngredient(
            std::make_unique<EndstoneItemTagIngredient>(std::move(recipe), ingredient, descriptor.getFullName()));
    case ItemDescriptor::InternalType::Molang:
        return MolangIngredient(
            std::make_unique<EndstoneMolangIngredient>(std::move(recipe), ingredient, descriptor.getFullName()));
    case ItemDescriptor::InternalType::ComplexAlias:
        return ComplexAliasIngredient(
            std::make_unique<EndstoneComplexAliasIngredient>(std::move(recipe), ingredient, descriptor.getFullName()));
    case ItemDescriptor::InternalType::Default: {
        const auto *type = endstone::ItemType::get(descriptor.getFullName());
        if (descriptor.getItem() == nullptr || type == nullptr) {
            return std::nullopt;
        }
        const auto aux_value = descriptor.getAuxValue();
        if (aux_value == ItemDescriptor::ANY_AUX_VALUE) {
            return ItemTypeIngredient(
                std::make_unique<EndstoneItemTypeIngredient>(std::move(recipe), ingredient, *type));
        }
        auto item = type->createItemStack(static_cast<int>(ingredient.getStackSize()));
        item.setData(aux_value);
        return ExactIngredient(std::make_unique<EndstoneExactIngredient>(std::move(recipe), ingredient, std::move(item)));
    }
    default:
        return std::nullopt;
    }
}

const ::RecipeIngredient &EndstoneIngredient::getHandle(const RecipeIngredient &ingredient)
{
    return static_cast<const EndstoneIngredient &>(*ingredient.impl_).getHandle();
}

::RecipeIngredient EndstoneIngredient::toMinecraft(const RecipeIngredient &ingredient)
{
    if (dynamic_cast<const EndstoneIngredient *>(ingredient.impl_.get()) != nullptr) {
        return getHandle(ingredient);
    }
    if (ingredient.is<ExactIngredient>()) {
        const auto exact = ingredient.as<ExactIngredient>();
        auto minecraft = EndstoneItemStack::toMinecraft(exact->getItemStack());
        if (const auto *item = minecraft.getItem()) {
            return ::RecipeIngredient(ItemDescriptor(*item, minecraft.getAuxValue()),
                                      static_cast<std::uint16_t>(minecraft.getCount()));
        }
        return {};
    }
    if (ingredient.is<ItemTypeIngredient>()) {
        const auto type = ingredient.as<ItemTypeIngredient>();
        return ::RecipeIngredient(std::string(type->getItemType().getId()), ItemDescriptor::ANY_AUX_VALUE,
                                  static_cast<std::uint16_t>(type->getCount()));
    }
    if (ingredient.is<ItemTagIngredient>()) {
        const auto tag = ingredient.as<ItemTagIngredient>();
        return ::RecipeIngredient(ItemTag(tag->getItemTag()), static_cast<std::uint16_t>(tag->getCount()));
    }
    if (ingredient.is<MolangIngredient>()) {
        const auto molang = ingredient.as<MolangIngredient>();
        return ::RecipeIngredient(molang->getExpression(), 0, static_cast<std::uint16_t>(molang->getCount()));
    }
    if (ingredient.is<ComplexAliasIngredient>()) {
        const auto alias = ingredient.as<ComplexAliasIngredient>();
        return ::RecipeIngredient(alias->getAlias(), ItemDescriptor::ANY_AUX_VALUE,
                                  static_cast<std::uint16_t>(alias->getCount()));
    }
    return {};
}

}  // namespace endstone::core
