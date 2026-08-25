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

#include "bedrock/world/item/crafting/recipes.h"

#include "bedrock/world/item/crafting/shaped_recipe.h"
#include "bedrock/world/item/item.h"

ItemInstance Recipes::getFurnaceRecipeResult(const ItemStackBase &item, const HashedString &tag) const
{
    if (!item) {
        return {};
    }

    const auto outer = furnace_results_.find(static_cast<int>(tag.getHash()));
    if (outer == furnace_results_.end()) {
        return {};
    }
    const auto &results = outer->second;

    if (const auto exact = results.find(item.getIdAux()); exact != results.end()) {
        return exact->second;
    }

    const auto any_id_aux =
        item.getItem()->buildIdAux(static_cast<std::int16_t>(ItemDescriptor::ANY_AUX_VALUE), item.getUserData());
    if (const auto any = results.find(any_id_aux); any != results.end()) {
        return any->second;
    }

    return {};
}

void Recipes::_addItemRecipe(std::unique_ptr<Recipe> recipe)
{
    if (!recipe) {
        return;
    }

    auto &by_tag = recipes_[recipe->getTag()];
    if (by_tag.contains(recipe->getRecipeId())) {
        return;
    }

    if (recipe->getNetId().raw_id == 0) {
        RecipeNetId net_id;
        net_id.raw_id = static_cast<unsigned int>(recipes_by_net_id_.size()) + 1;
        while (recipes_by_net_id_.contains(net_id)) {
            ++net_id.raw_id;
        }
        recipe->setNetId(net_id);
    }
    else if (recipes_by_net_id_.contains(recipe->getNetId())) {
        return;
    }

    const auto recipe_id = recipe->getRecipeId();
    auto shared = std::shared_ptr<Recipe>(std::move(recipe));
    const auto inserted = by_tag.emplace(recipe_id, std::move(shared));
    auto *handle = inserted.first->second.get();
    recipes_by_net_id_[handle->getNetId()] = handle;

    if (handle->getUnlockingRequirement().isUnlockable()) {
        unlockable_recipes_.emplace_back(handle);
        unique_unlockable_recipe_ids_.emplace(handle->getRecipeId());
    }

    if (handle->hasDataDrivenResult()) {
        for (const auto &result : handle->getResultItems()) {
            if (result.isNull() || result.getCount() == 0) {
                continue;
            }
            recipes_by_output_[result][handle->getRecipeId()] = handle;

            const auto *item = result.getItem();
            if (item == nullptr) {
                continue;
            }
            auto &by_input = recipes_by_input_[handle->getTag().getHash()];
            auto add_input = [&](std::size_t input_hash) {
                auto &outputs = by_input[input_hash];
                if (!outputs) {
                    outputs = std::make_shared<std::vector<ItemInstance>>();
                }
                outputs->push_back(result);
            };
            if (handle->isShapeless()) {
                add_input(handle->getIngredientsHash());
                continue;
            }
            const auto *shaped = static_cast<const ShapedRecipe *>(handle);
            const int width = handle->getWidth();
            const int height = handle->getHeight();
            if (width >= 4 || height >= 4) {
                continue;
            }
            for (int simulated_height = 1; simulated_height <= 3; ++simulated_height) {
                for (int simulated_width = 1; simulated_width <= 3; ++simulated_width) {
                    for (int offset_x = 0; offset_x <= simulated_width - width; ++offset_x) {
                        for (int offset_y = 0; offset_y <= simulated_height - height; ++offset_y) {
                            add_input(shaped->getIngredientsHashOffset(simulated_width, simulated_height, offset_x,
                                                                       offset_y));
                        }
                    }
                }
            }
        }
    }

    for (auto &[alive, listener] : listeners_) {
        if (!alive.expired()) {
            listener();
        }
    }
}

bool Recipes::removeRecipe(const std::string &recipe_id)
{
    ::Recipe *handle = nullptr;
    for (auto &[tag, by_id] : recipes_) {
        const auto it = by_id.find(recipe_id);
        if (it == by_id.end()) {
            continue;
        }
        handle = it->second.get();
        recipes_by_net_id_.erase(handle->getNetId());
        unique_unlockable_recipe_ids_.erase(recipe_id);
        std::erase_if(unlockable_recipes_, [handle](Recipe *entry) { return entry == handle; });
        for (auto output = recipes_by_output_.begin(); output != recipes_by_output_.end();) {
            output->second.erase(recipe_id);
            if (output->second.empty()) {
                output = recipes_by_output_.erase(output);
            }
            else {
                ++output;
            }
        }
        by_id.erase(it);
        return true;
    }
    return false;
}
