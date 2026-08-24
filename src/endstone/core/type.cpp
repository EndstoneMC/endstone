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

#include "endstone/core/type.h"

#include <unordered_map>

#include <entt/meta/factory.hpp>
#include <entt/meta/resolve.hpp>

#include "endstone/actor/actor.h"
#include "endstone/actor/item.h"
#include "endstone/actor/mob.h"
#include "endstone/block/block_state.h"
#include "endstone/block/campfire.h"
#include "endstone/block/container.h"
#include "endstone/block/creature_spawner.h"
#include "endstone/block/furnace.h"
#include "endstone/block/item_frame.h"
#include "endstone/block/lectern.h"
#include "endstone/block/sign.h"
#include "endstone/command/block_command_sender.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/command/proxied_command_sender.h"
#include "endstone/inventory/complex_alias_ingredient.h"
#include "endstone/inventory/complex_recipe.h"
#include "endstone/inventory/exact_ingredient.h"
#include "endstone/inventory/item_tag_ingredient.h"
#include "endstone/inventory/item_type_ingredient.h"
#include "endstone/inventory/meta/book_meta.h"
#include "endstone/inventory/meta/crossbow_meta.h"
#include "endstone/inventory/meta/item_meta.h"
#include "endstone/inventory/meta/map_meta.h"
#include "endstone/inventory/meta/potion_meta.h"
#include "endstone/inventory/meta/writable_book_meta.h"
#include "endstone/inventory/molang_ingredient.h"
#include "endstone/inventory/recipe.h"
#include "endstone/inventory/recipe_ingredient.h"
#include "endstone/inventory/shaped_recipe.h"
#include "endstone/inventory/shapeless_recipe.h"
#include "endstone/inventory/smithing_recipe.h"
#include "endstone/inventory/smithing_transform_recipe.h"
#include "endstone/inventory/smithing_trim_recipe.h"
#include "endstone/permissions/permissible.h"
#include "endstone/player.h"

namespace endstone::core {

namespace {
std::unordered_map<ClassInfo, entt::meta_type> MetaLookup;

template <typename T>
entt::meta_factory<T> registerType()
{
    auto factory = entt::meta_factory<T>{};
    MetaLookup[typeid(T)] = entt::resolve<T>();
    return factory;
}
}  // namespace

void registerTypes()
{
    // Permissible / CommandSender / Actor hierarchy
    registerType<Permissible>();
    registerType<CommandSender>().base<Permissible>();
    registerType<ConsoleCommandSender>().base<CommandSender>();
    registerType<BlockCommandSender>().base<CommandSender>();
    registerType<ProxiedCommandSender>().base<CommandSender>();
    registerType<Actor>().base<CommandSender>();
    registerType<Mob>().base<Actor>();
    registerType<Player>().base<Mob>();
    registerType<Item>().base<Actor>();

    // ItemMeta hierarchy
    registerType<ItemMeta>();
    registerType<MapMeta>().base<ItemMeta>();
    registerType<WritableBookMeta>().base<ItemMeta>();
    registerType<BookMeta>().base<WritableBookMeta>();
    registerType<CrossbowMeta>().base<ItemMeta>();
    registerType<PotionMeta>().base<ItemMeta>();

    // BlockState hierarchy
    registerType<BlockState>();
    registerType<Container>().base<BlockState>();
    registerType<Furnace>().base<Container>();
    registerType<Lectern>().base<Container>();
    registerType<Campfire>().base<BlockState>();
    registerType<CreatureSpawner>().base<BlockState>();
    registerType<ItemFrame>().base<BlockState>();
    registerType<Sign>().base<BlockState>();

    // Recipe hierarchy
    registerType<Recipe>();
    registerType<ComplexRecipe>().base<Recipe>();
    registerType<ShapedRecipe>().base<Recipe>();
    registerType<ShapelessRecipe>().base<Recipe>();
    registerType<SmithingRecipe>().base<Recipe>();
    registerType<SmithingTransformRecipe>().base<SmithingRecipe>();
    registerType<SmithingTrimRecipe>().base<SmithingRecipe>();

    // RecipeIngredient hierarchy
    registerType<RecipeIngredient>();
    registerType<ExactIngredient>().base<RecipeIngredient>();
    registerType<ComplexAliasIngredient>().base<RecipeIngredient>();
    registerType<ItemTagIngredient>().base<RecipeIngredient>();
    registerType<ItemTypeIngredient>().base<RecipeIngredient>();
    registerType<MolangIngredient>().base<RecipeIngredient>();
}

bool isTypeInstanceOf(ClassInfo from, ClassInfo target)
{
    if (from == target) {
        return true;
    }
    auto it_from = MetaLookup.find(from);
    auto it_to = MetaLookup.find(target);
    if (it_from == MetaLookup.end() || it_to == MetaLookup.end()) {
        return false;
    }
    return it_from->second.can_cast(it_to->second);
}

bool isInstanceOf(const Object &obj, ClassInfo target)
{
    return isTypeInstanceOf(obj.getClassInfo(), target);
}

}  // namespace endstone::core
