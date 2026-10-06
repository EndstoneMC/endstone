#include "bedrock/world/level/block/liquid_block.h"

#include <memory>

#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/material/material.h"
#include "endstone/block/block_state.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_snapshot.h"
#include "endstone/core/block/block_type.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_form_event.h"
#include "endstone/event/block/block_from_to_event.h"
#include "endstone/runtime/hook.h"

void LiquidBlockBase::_solidify(BlockSource &region, BlockPos const &pos, BlockPos const &changed_neighbor) const
{
    if (&region.getBlock(pos).getBlockType() != this || changed_neighbor.y < pos.y ||
        !getMaterial().isType(MaterialType::Lava)) {
        return;
    }

    const HashedString *formed;
    if (region.getLiquidBlock(changed_neighbor).getMaterial().isType(MaterialType::Water)) {
        formed = region.getBlock(pos).getState<int>(VanillaStateIds::LiquidDepth) != 0
                   ? &VanillaBlockTypeIds::Cobblestone
                   : &VanillaBlockTypeIds::Obsidian;
    }
    else if (region.getBlock(changed_neighbor).getBlockType().getName() == VanillaBlockTypeIds::BlueIce &&
             region.getBlock({pos.x, pos.y - 1, pos.z}).getBlockType().getName() == VanillaBlockTypeIds::SoulSoil) {
        formed = &VanillaBlockTypeIds::Basalt;
    }
    else {
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto *placed =
        &static_cast<const endstone::core::EndstoneBlockType &>(
             server.getRegistry<endstone::BlockType>().getOrThrow(endstone::BlockTypeId(formed->getString())))
             .getHandle()
             .getDefaultState();

    // Endstone begins
    if (server.getEndstonePluginManager().isEventRegistered<endstone::BlockFormEvent>() &&
        !region.isInstaticking(pos)) {
        const auto new_state =
            std::make_shared<endstone::core::EndstoneBlockSnapshot>(region, pos, *placed)->captureState(true);
        endstone::BlockFormEvent event{endstone::core::EndstoneBlock::at(region, pos), new_state};
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return;
        }
        placed = &static_cast<const endstone::core::EndstoneBlockData &>(*new_state->getData()).getHandle();
    }
    // Endstone ends

    region.setBlock(pos, *placed, UPDATE_ALL, nullptr, {});
    if (!region.isInstaticking(pos)) {
        emitFizzParticle(region, pos);
    }
}

void LiquidBlock::_trySpreadTo(BlockSource &region, BlockPos const &pos, int neighbor, BlockPos const &flow_from_pos,
                               FacingID flow_from_direction) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (server.getEndstonePluginManager().isEventRegistered<endstone::BlockFromToEvent>() &&
        _canSpreadTo(region, pos, flow_from_pos, flow_from_direction) && !region.isInstaticking(pos)) {
        endstone::BlockFromToEvent event(endstone::core::EndstoneBlock::at(region, flow_from_pos),
                                         endstone::core::EndstoneBlock::at(region, pos));
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&LiquidBlock::_trySpreadTo, this, region, pos, neighbor, flow_from_pos,
                                flow_from_direction);
}
