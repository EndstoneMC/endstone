#include "bedrock/world/level/block/actor/campfire_block_actor.h"

#include "bedrock/world/events/gameevents/game_event.h"
#include "bedrock/world/level/block/furnace_types.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/recipe_data.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_cook_event.h"
#include "endstone/inventory/cooking_recipe.h"

namespace {
endstone::Nullable<endstone::CookingRecipe> findCookingRecipe(const Recipes &recipes, const ItemStack &input,
                                                              const HashedString &tag)
{
    const auto &all_tags = recipes.getRecipesAllTags();
    const auto by_tag = all_tags.find(tag);
    if (by_tag == all_tags.end()) {
        return nullptr;
    }
    for (const auto &by_id : by_tag->second) {
        const auto &recipe = by_id.second;
        if (!recipe || recipe->getIngredients().empty()) {
            continue;
        }
        if (recipe->itemValidForRecipe(recipe->getIngredients().front(), input)) {
            return endstone::core::EndstoneRecipeData::fromMinecraft(recipe).as<endstone::CookingRecipe>();
        }
    }
    return nullptr;
}
}  // namespace

void CampfireBlockActor::_finishCooking(::BlockSource &region, int slot)
{
    if (region.canDoBlockDrops(nullptr)) {
        const auto &block = region.getBlock(this->position_);
        auto tag = CAMPFIRE_TAG;
        if (block.getName() == VanillaBlockTypeIds::SoulCampfire) {
            tag = SOUL_CAMPFIRE_TAG;
        }

        auto source = ItemStack(cooking_item_[slot]);
        const auto &recipes = region.getLevel().getRecipes();
        auto result = ItemStack(recipes.getFurnaceRecipeResult(cooking_item_[slot], tag));
        if (result.getAuxValue() == ItemDescriptor::ANY_AUX_VALUE) {
            result.setAuxValue(0);
        }

        // Endstone start
        const auto &server = endstone::core::EndstoneServer::getInstance();
        if (server.getEndstonePluginManager().isEventRegistered<endstone::BlockCookEvent>()) {
            endstone::BlockCookEvent event{endstone::core::EndstoneBlock::at(region, position_),
                                           endstone::core::EndstoneItemStack::fromMinecraft(source),
                                           endstone::core::EndstoneItemStack::fromMinecraft(result),
                                           findCookingRecipe(recipes, source, tag)};
            server.getPluginManager().callEvent(event);
            if (event.isCancelled()) {
                return;
            }
            result = endstone::core::EndstoneItemStack::toMinecraft(event.getResult());
        }
        // Endstone end

        region.getLevel().getSpawner().spawnItem(region, result, nullptr, Vec3(position_) + 0.5, 10);
        region.postGameEvent(nullptr, GameEventRegistry::blockChange, position_, nullptr);
        cooking_item_[slot].setNull(std::nullopt);
        cooking_time_[slot] = 0;
        VanillaBlockActor::setChanged();
    }
}
