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

#include "endstone/core/level/level.h"

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/core/utility/automatic_id.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/level/dimension.h"
#include "endstone/inventory/recipes/shaped_recipe.h"
#include "endstone/inventory/recipes/shapeless_recipe.h"
#include "endstone/level/dimension.h"

namespace endstone::core {

EndstoneLevel::EndstoneLevel(::Level &level) : server_(entt::locator<EndstoneServer>::value()), level_(level)
{
    // Load all dimensions when the level is loaded
    static constexpr AutomaticID<::Dimension, int> dimension_ids[] = {
        VanillaDimensions::Overworld, VanillaDimensions::Nether, VanillaDimensions::TheEnd};
    for (const auto &dimension_id : dimension_ids) {
        auto dimension = level.getOrCreateDimension(dimension_id);
        addDimension(std::make_unique<EndstoneDimension>(*dimension.unwrap(), *this));
    }
}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
}

std::vector<Actor *> EndstoneLevel::getActors() const
{
    std::vector<Actor *> result;
    for (const auto &entity : level_.getEntities()) {
        if (!entity.hasValue()) {
            continue;
        }
        const auto *actor = ::Actor::tryGetFromEntity(*entity, false);
        if (!actor) {
            continue;
        }
        if (&actor->getLevel() != &level_) {
            continue;
        }
        result.push_back(&actor->getEndstoneActor());
    }
    return result;
}

int EndstoneLevel::getTime() const
{
    return level_.getTime();
}

void EndstoneLevel::setTime(int time)
{
    level_.setTime(time);
}

std::vector<Dimension *> EndstoneLevel::getDimensions() const
{
    std::vector<Dimension *> dimensions;
    dimensions.reserve(dimensions_.size());
    for (const auto &it : dimensions_) {
        dimensions.push_back(it.second.get());
    }
    return dimensions;
}

Dimension *EndstoneLevel::getDimension(std::string name) const
{
    if (name == "the_end") {
        name = "TheEnd";
    }
    std::ranges::transform(name, name.begin(), [](unsigned char c) { return std::tolower(c); });
    const auto it = dimensions_.find(name);
    if (it == dimensions_.end()) {
        return nullptr;
    }
    return it->second.get();
}

void EndstoneLevel::addDimension(std::unique_ptr<Dimension> dimension)
{
    auto name = dimension->getName();
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    if (dimensions_.find(name) != dimensions_.end()) {
        server_.getLogger().error(
            "Dimension {} is a duplicate of another dimension and has been prevented from loading.", name);
        return;
    }
    dimensions_[name] = std::move(dimension);
}

EndstoneServer &EndstoneLevel::getServer() const
{
    return server_;
}

::Level &EndstoneLevel::getHandle() const
{
    return level_;
}

void EndstoneLevel::addRecipe(std::shared_ptr<endstone::Recipe> recipe)
{
    auto recipes = level_.getRecipes();
    switch (recipe->getType()) {
    case RecipeType::Shaped: {
        auto *rec = reinterpret_cast<ShapedRecipe *>(recipe.get());
        std::vector<::ItemInstance> results;
        for (auto &result : rec->getResult()) {
            results.emplace_back(EndstoneItemStack::toMinecraft(result.get()));
        }
        std::vector<::Recipes::Type> types;
        for (const auto &[c, ingredient] : rec->getIngredientMap()) {
            ::Recipes::Type type;
            auto stack = EndstoneItemStack::toMinecraft(ingredient->getChoice().get());
            type.ingredient = std::move(::RecipeIngredient{*stack.getItem(), stack.getAuxValue(), 1});
            type.c = c;
            types.emplace_back(std::move(type));
        }

        recipes.addShapedRecipe(
            recipe->getId(), results, rec->shape(), types, {}, 0, nullptr,
            RecipeUnlockingRequirement{RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked},
            SemVersion(1, 21, 0), false);
        break;
    }
    case RecipeType::Shapeless: {
        auto *rec = reinterpret_cast<ShapelessRecipe *>(recipe.get());
        ::ItemInstance result{EndstoneItemStack::toMinecraft(rec->getResult()[0].get())};
        std::vector<::Recipes::Type> types;
        for (const auto &[ingredient, c] : rec->getIngredients()) {
            ::Recipes::Type type;
            auto stack = EndstoneItemStack::toMinecraft(ingredient->getChoice().get());
            type.ingredient = std::move(::RecipeIngredient{*stack.getItem(), stack.getAuxValue(), c});
            types.emplace_back(std::move(type));
        }

        recipes.addShapelessRecipe(
            recipe->getId(), result, types, {}, 0, nullptr,
            RecipeUnlockingRequirement{RecipeUnlockingRequirement::UnlockingContext::AlwaysUnlocked},
            SemVersion(1, 21, 0));
        break;
    }
    }
}

};  // namespace endstone::core
