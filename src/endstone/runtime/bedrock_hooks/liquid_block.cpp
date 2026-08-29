#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/block/block_state.h"
#include "endstone/event/block/block_form_event.h"
#include "endstone/event/block/block_from_to_event.h"
#include "endstone/runtime/hook.h"

void LiquidBlockBase::_solidify(BlockSource &region, BlockPos const &pos, BlockPos const &changed_neighbor) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::BlockFormEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&LiquidBlockBase::_solidify, this, region, pos, changed_neighbor);
        return;
    }

    const auto &old_block = region.getBlock(pos);
    const auto old_runtime_id = old_block.getRuntimeId();
    ENDSTONE_HOOK_CALL_ORIGINAL(&LiquidBlockBase::_solidify, this, region, pos, changed_neighbor);

    const auto &formed_block = region.getBlock(pos);
    if (formed_block.getRuntimeId() == old_runtime_id) {
        return;
    }

    auto block_handle = endstone::core::EndstoneBlock::at(region, pos);
    auto new_state = block_handle->captureState(true);
    endstone::BlockFormEvent event(block_handle, new_state);
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        region.setBlock(pos, old_block, ::BlockType::UPDATE_ALL, nullptr, {});
        return;
    }

    const auto &final_block =
        static_cast<const endstone::core::EndstoneBlockData &>(*event.getNewState()->getData()).getHandle();
    if (final_block.getRuntimeId() != formed_block.getRuntimeId()) {
        region.setBlock(pos, final_block, ::BlockType::UPDATE_ALL, nullptr, {});
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
