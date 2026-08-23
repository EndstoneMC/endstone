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

#include "endstone/core/inventory/furnace_recipe.h"

#include <cstdint>
#include <memory>

#include "bedrock/world/item/crafting/recipes.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_descriptor.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/inventory/exact_ingredient.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/item_type_ingredient.h"

namespace endstone::core {
namespace {

template <typename Interface>
class EndstoneFurnaceIngredientBase : public Interface {
public:
    explicit EndstoneFurnaceIngredientBase(int id_aux) : id_aux_(id_aux) {}

    [[nodiscard]] const std::type_info &getClassTypeId() const override { return typeid(Interface); }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return core::isInstanceOf(*this, target);
    }

    [[nodiscard]] int getCount() const override { return 1; }

protected:
    int id_aux_;
};

class EndstoneFurnaceExactIngredient final : public EndstoneFurnaceIngredientBase<endstone::ExactIngredient> {
public:
    EndstoneFurnaceExactIngredient(int id_aux, endstone::ItemStack item)
        : EndstoneFurnaceIngredientBase(id_aux), item_(std::move(item))
    {
    }

    [[nodiscard]] bool test(const endstone::ItemStack &item) const override
    {
        const auto minecraft_item = EndstoneItemStack::toMinecraft(item);
        return minecraft_item && minecraft_item.getIdAux() == id_aux_;
    }

    [[nodiscard]] endstone::ItemStack getItemStack() const override { return item_; }

private:
    endstone::ItemStack item_;
};

class EndstoneFurnaceItemTypeIngredient final : public EndstoneFurnaceIngredientBase<endstone::ItemTypeIngredient> {
public:
    EndstoneFurnaceItemTypeIngredient(int id_aux, const endstone::ItemType &type)
        : EndstoneFurnaceIngredientBase(id_aux), type_(&type)
    {
    }

    [[nodiscard]] bool test(const endstone::ItemStack &item) const override
    {
        const auto minecraft_item = EndstoneItemStack::toMinecraft(item);
        return minecraft_item &&
               minecraft_item.getItem()->buildIdAux(static_cast<std::int16_t>(ItemDescriptor::ANY_AUX_VALUE),
                                                    minecraft_item.getUserData()) == id_aux_;
    }

    [[nodiscard]] const endstone::ItemType &getItemType() const override { return *type_; }

private:
    const endstone::ItemType *type_;
};

}  // namespace

Nullable<endstone::FurnaceRecipe> EndstoneFurnaceRecipe::fromMinecraft(const int input_id_aux,
                                                                       const ::ItemInstance &result, std::string tag)
{
    const auto item_id = static_cast<std::int16_t>(input_id_aux >> 16);
    const auto input_aux = static_cast<std::uint16_t>(input_id_aux);
    const auto item = ItemRegistryManager::getItemRegistry().getItem(item_id);
    if (item.isNull()) {
        return nullptr;
    }

    const auto *type = endstone::ItemType::get(item->getFullItemName());
    if (type == nullptr) {
        return nullptr;
    }

    std::shared_ptr<endstone::RecipeIngredient> input;
    if (input_aux == ItemDescriptor::ANY_AUX_VALUE) {
        input = std::make_shared<EndstoneFurnaceItemTypeIngredient>(input_id_aux, *type);
    }
    else {
        auto item_stack = EndstoneItemStack::fromMinecraft(::ItemStack(*item.get(), 1, input_aux));
        input = std::make_shared<EndstoneFurnaceExactIngredient>(input_id_aux, std::move(item_stack));
    }

    auto endstone_result = EndstoneItemStack::fromMinecraft(result);
    if (endstone_result.getData() == ItemDescriptor::ANY_AUX_VALUE) {
        endstone_result.setData(0);
    }
    return std::make_shared<EndstoneFurnaceRecipe>(std::move(endstone_result), std::move(tag), std::move(input));
}

Nullable<endstone::FurnaceRecipe> EndstoneFurnaceRecipe::fromMinecraft(const ::Recipes &recipes,
                                                                       const ::ItemStackBase &input,
                                                                       const ::HashedString &tag)
{
    const auto *results = recipes.findFurnaceResults(tag);
    if (results == nullptr || !input) {
        return nullptr;
    }

    if (const auto exact = results->find(input.getIdAux()); exact != results->end()) {
        return fromMinecraft(exact->first, exact->second, tag.getString());
    }

    const auto any_id_aux =
        input.getItem()->buildIdAux(static_cast<std::int16_t>(ItemDescriptor::ANY_AUX_VALUE), input.getUserData());
    if (const auto any = results->find(any_id_aux); any != results->end()) {
        return fromMinecraft(any->first, any->second, tag.getString());
    }

    return nullptr;
}

}  // namespace endstone::core
