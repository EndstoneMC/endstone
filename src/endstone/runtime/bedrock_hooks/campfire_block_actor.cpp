#include "bedrock/world/level/block/actor/campfire_block_actor.h"

#include "bedrock/world/events/gameevents/game_event.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/block/furnace_types.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/spawner.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_cook_event.h"

void CampfireBlockActor::_finishCooking(::BlockSource &region, int slot)
{
    if (region.canDoBlockDrops()) {
        const auto &block = region.getBlock(this->position_);
        auto tag = CAMPFIRE_TAG;
        if (block.getName() == VanillaBlockTypeIds::SoulCampfire) {
            tag = SOUL_CAMPFIRE_TAG;
        }

        auto source = ItemStack(cooking_item_[slot]);
        auto result = ItemStack(region.getLevel().getRecipes().getFurnaceRecipeResult(cooking_item_[slot], tag));
        if (result.getAuxValue() == ItemDescriptor::ANY_AUX_VALUE) {
            result.setAuxValue(0);
        }

        // Endstone start
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::BlockCookEvent event{endstone::core::EndstoneBlock::at(region, position_),
                                       endstone::core::EndstoneItemStack::fromMinecraft(source),
                                       endstone::core::EndstoneItemStack::fromMinecraft(result)};
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return;
        }
        result = endstone::core::EndstoneItemStack::toMinecraft(event.getResult());
        // Endstone end

        region.getLevel().getSpawner().spawnItem(region, result, nullptr, Vec3(position_) + 0.5, 10);
        region.postGameEvent(nullptr, GameEventRegistry::blockChange, position_, nullptr);
        cooking_item_[slot].setNull(std::nullopt);
        cooking_time_[slot] = 0;
        setChanged();
    }
}
