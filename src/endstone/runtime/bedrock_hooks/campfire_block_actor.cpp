#include "bedrock/world/level/block/actor/campfire_block_actor.h"

#include <iostream>

#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/block/furnace_types.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/spawner.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_cook_event.h"
#include "endstone/runtime/hook.h"

void CampfireBlockActor::_finishCooking(::BlockSource &region, int slot)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CampfireBlockActor::_finishCooking, this, region, slot);

    if (region.canDoBlockDrops()) {
        const auto &block = region.getBlock(this->position_);
        HashedString tag = CAMPFIRE_TAG;
        if (block.getName() == BedrockBlockNames::SoulCampFire) {
            tag = SOUL_CAMPFIRE_TAG;
        }
        auto result = region.getLevel().getRecipes().getFurnaceRecipeResult(cooking_item_[slot], tag);
        auto aux = result.getAuxValue();
        if (result.isBlock() && result.getAuxValue() != 0x7FFF) {
            result.setAuxValue(result.getAuxValue());
        }
        if (result.getAuxValue() == 0x7FFF) {
            result.setAuxValue(0);
        }
        const auto &server = endstone::core::EndstoneServer::getInstance();

        endstone::BlockCookEvent event{
            endstone::core::EndstoneBlock::at(region, position_),
            *endstone::core::EndstoneItemStack::fromMinecraft(
                *reinterpret_cast<const ItemStack *>(&cooking_item_[slot])),
            *endstone::core::EndstoneItemStack::fromMinecraft(*reinterpret_cast<const ItemStack *>(&result))};

        std::cout << event.getSource().getType().getKey().toString() << "\n";
        std::cout << event.getResult().getType().getKey().toString() << "\n";

        server.getPluginManager().callEvent(event);

        if (event.isCancelled()) {
            return;
        }

        region.getLevel().getSpawner().spawnItem(region, *reinterpret_cast<const ItemStack *>(&result), nullptr,
                                                 {.x = static_cast<float>(position_.x + 0.5),
                                                  .y = static_cast<float>(position_.y + 0.5),
                                                  .z = static_cast<float>(position_.z + 0.5)},
                                                 10);
        // todo: postGameEvent
        cooking_item_[slot].setNull(std::nullopt);
        cooking_time_[slot] = 0;
        this->changed_ = true;
    }
}
