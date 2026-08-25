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

#include "bedrock/world/item/crafting/recipe.h"

#include <random>

#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/item_stack.h"

Recipe::Results::Results(const ResultList &results) : results_are_loaded_(true), results_(results) {}

Recipe::Results::Results(const UnloadedItemInstanceResultList &results)
    : results_are_loaded_(false), unloaded_results_(results)
{
}

Recipe::Recipe(ConstructionContext &&context)
    : recipe_id_(std::move(context.recipe_id)), my_ingredients_(std::move(context.ingredients)),
      results_(std::move(context.results)), unlocking_requirement_(context.unlocking_requirement),
      recipe_data_version_(std::move(context.format_version)), tag_(std::move(context.tag))
{
    width_ = 0;
    height_ = 0;
    priority_ = context.priority;
    if (context.uuid != nullptr) {
        my_id_ = *context.uuid;
    }
    else {
        generateUUID();
    }
}

Recipe::Recipe() : tag_(nullptr) {}

const mce::UUID &Recipe::getId() const
{
    return my_id_;
}

const Recipe::ResultList &Recipe::getResultItems() const
{
    return results_.getItems();
}

bool Recipe::isMultiRecipe() const
{
    return false;
}

bool Recipe::hasDataDrivenResult() const
{
    return true;
}

bool Recipe::isAnyAuxValue(const ItemDescriptor &ii)
{
    if (!ii.isValid(true) || ii.getItem() == nullptr) {
        return false;
    }
    return ii.getAuxValue() == ItemDescriptor::ANY_AUX_VALUE;
}

bool Recipe::itemValidForRecipe(const ItemDescriptor &recipe_item, const ItemStack &item_stack) const
{
    if (const auto *user_data = item_stack.getUserData()) {
        if (const auto *ench = user_data->get(ItemStackBase::TAG_ENCHANTS);
            ench != nullptr && ench->getId() == Tag::Type::Int64) {
            return false;
        }
        if (user_data->contains("minecraft:item_lock") && user_data->getByte("minecraft:item_lock") != 0) {
            return false;
        }
    }
    const auto descriptor = item_stack.getDescriptor();
    return itemsMatch(recipe_item, descriptor, item_stack.getUserData());
}

bool Recipe::itemsMatch(const ItemDescriptor &lhs, const ItemDescriptor &rhs) const
{
    const bool lhs_valid = lhs.isValid(true);
    const bool rhs_valid = rhs.isValid(true);
    if (!lhs_valid && !rhs_valid) {
        return true;
    }
    if (!lhs_valid || !rhs_valid) {
        return false;
    }
    return lhs.sameItem(rhs, true);
}

bool Recipe::itemsMatch(const ItemDescriptor &lhs, const ItemDescriptor &rhs, const CompoundTag *rhs_tag) const
{
    const bool lhs_valid = lhs.isValid(true);
    const bool rhs_valid = rhs.isValid(true);
    if (!lhs_valid && !rhs_valid && rhs_tag == nullptr) {
        return true;
    }
    if (!lhs_valid || !rhs_valid) {
        return false;
    }
    return lhs.sameItem(rhs, true);
}

std::size_t Recipe::getIngredientsHash() const
{
    std::size_t hash = 0;
    for (const auto &ingredient : my_ingredients_) {
        hash += static_cast<const ItemDescriptor &>(ingredient).getHash();
    }
    return hash;
}

void Recipe::generateUUID()
{
    std::random_device device;
    const auto a = device();
    const auto b = device();
    const auto c = device();
    const auto d = device();
    const std::uint64_t high = (static_cast<std::uint64_t>(c) << 32) | (d & 0xFFFF0FFFu) | 0x4000u;
    const std::uint64_t low =
        ((static_cast<std::uint64_t>(a) << 32) & 0x3FFFFFFFFF000000ULL) | b | 0x8000000000000000ULL;
    my_id_ = {high, low};
}

Recipe::ConstructionContext Recipe::getConstructionContext() const
{
    ConstructionContext context;
    context.recipe_id = recipe_id_;
    context.ingredients = my_ingredients_;
    context.results = results_;
    context.tag = tag_;
    context.priority = priority_;
    context.uuid = &my_id_;
    context.unlocking_requirement = unlocking_requirement_;
    context.format_version = recipe_data_version_;
    return context;
}
