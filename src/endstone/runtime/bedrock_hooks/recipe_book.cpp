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

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "bedrock/bedrock.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/inventory/network/item_stack_request_action.h"
#include "bedrock/world/inventory/network/item_stack_net_result.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_recipe_book_click_event.h"
#include "endstone/runtime/hook.h"

extern "C" ENDSTONE_HOOK ItemStackNetResult handleCraftAction(
    void *handler, const ItemStackRequestActionCraftBase &request_action);

namespace {
template <typename T>
[[nodiscard]] void *getVtable(const T &object)
{
    static_assert(std::is_polymorphic_v<T>);

    void *vtable;
    std::memcpy(&vtable, static_cast<const void *>(&object), sizeof(vtable));
    return vtable;
}

template <typename T>
void setVtable(T &object, void *vtable)
{
    static_assert(std::is_polymorphic_v<T>);

    std::memcpy(static_cast<void *>(&object), &vtable, sizeof(vtable));
}

template <typename T>
class ScopedBdsVtable final {
public:
    ScopedBdsVtable(T &object, void *vtable) : object_(object), original_(getVtable(object)) { setVtable(object_, vtable); }

    ScopedBdsVtable(const ScopedBdsVtable &) = delete;
    ScopedBdsVtable &operator=(const ScopedBdsVtable &) = delete;

    ~ScopedBdsVtable() { setVtable(object_, original_); }

private:
    T &object_;
    void *original_;
};

ItemStackRequestActionCraftRecipe makeCraftRecipeAction(const ItemStackRequestActionCraftRecipe &source)
{
    ItemStackRequestActionCraftRecipe action{};
    const auto vtable = getVtable(action);
    std::memcpy(static_cast<void *>(&action), static_cast<const void *>(&source), sizeof(action));
    setVtable(action, vtable);
    action.type_ = ItemStackRequestActionType::CraftingRecipe;
    action.craft_count_ = 1;
    return action;
}

ItemStackRequestActionCraftRecipeAuto makeCraftRecipeAutoAction(std::uint8_t craft_count, const ::Recipe &recipe)
{
    ItemStackRequestActionCraftRecipeAuto action{};
    action.type_ = ItemStackRequestActionType::CraftingRecipeAuto;
    action.craft_count_ = craft_count;
    action.recipe_net_id_ = recipe.getNetId();
    const auto &ingredients = recipe.getIngredients();
    action.ingredients_.reserve(ingredients.size());
    for (const auto &ingredient : ingredients) {
        action.ingredients_.emplace_back(ingredient);
    }
    action.ingredient_count_ = static_cast<std::uint8_t>(action.ingredients_.size());
    return action;
}

ItemStackNetResult callOriginal(ItemStackRequestActionHandler &handler,
                                const ItemStackRequestActionCraftBase &action)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&handleCraftAction, "handleCraftAction", &handler, action);
}

ItemStackNetResult dispatchCraftAction(ItemStackRequestActionHandler &handler,
                                       const ItemStackRequestActionCraftBase &action)
{
    const auto action_type = action.type_;
    if (action_type != ItemStackRequestActionType::CraftingRecipe &&
        action_type != ItemStackRequestActionType::CraftingRecipeAuto) {
        return callOriginal(handler, action);
    }

    const auto &recipe_action = static_cast<const ItemStackRequestActionCraftRecipe &>(action);
    const auto craft_count = action.craft_count_;
    const auto make_all = action_type == ItemStackRequestActionType::CraftingRecipeAuto || craft_count > 1;
    const auto *player = handler.getPlayer();
    auto &recipes = player->getLevel().getRecipes();
    const auto *native_recipe = recipes.getRecipeByNetId(recipe_action.recipe_net_id_);
    if (native_recipe == nullptr) {
        return callOriginal(handler, action);
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerRecipeBookClickEvent event{
        player->getEndstoneActor<endstone::core::EndstonePlayer>(), endstone::RecipeId{native_recipe->getRecipeId()},
        make_all};
    server.getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        return ItemStackNetResult::Error;
    }

    if (event.isMakeAll() == make_all) {
        return callOriginal(handler, action);
    }

    if (event.isMakeAll()) {
        auto auto_action = makeCraftRecipeAutoAction(craft_count, *native_recipe);
#ifdef _WIN32
        const auto auto_action_vtable = BEDROCK_VAR(void *, "??_7ItemStackRequestActionCraftRecipeAuto@@6B@");
#else
        const auto auto_action_vtable = BEDROCK_VAR(void *, "_ZTV37ItemStackRequestActionCraftRecipeAuto");
#endif
        ScopedBdsVtable vtable{auto_action, auto_action_vtable};
        return callOriginal(handler, auto_action);
    }

    auto normal_action = makeCraftRecipeAction(recipe_action);
    ScopedBdsVtable vtable{normal_action, getVtable(action)};
    return callOriginal(handler, normal_action);
}
}  // namespace

extern "C" ENDSTONE_HOOK ItemStackNetResult handleCraftAction(
    void *handler, const ItemStackRequestActionCraftBase &request_action)
{
    return dispatchCraftAction(*static_cast<ItemStackRequestActionHandler *>(handler), request_action);
}
