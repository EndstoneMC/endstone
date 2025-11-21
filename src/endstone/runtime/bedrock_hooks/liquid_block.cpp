#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_form_to_event.h"
#include "endstone/runtime/hook.h"

void LiquidBlock::_trySpreadTo(BlockSource &region, BlockPos const &pos, int neighbor, BlockPos const &flow_from_pos,
                               unsigned char flow_from_direction) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (server.isPrimaryThread()) {
        if (!_canSpreadTo(region, pos, flow_from_pos, flow_from_direction)) {
            return;
        }
        endstone::BlockFormToEvent event(endstone::core::EndstoneBlock::at(region, flow_from_pos),
                                         endstone::core::EndstoneBlock::at(region, pos));
        server.getPluginManager().callEvent(event);
        if (event.isCancelled()) {
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&LiquidBlock::_trySpreadTo, this, region, pos, neighbor, flow_from_pos,
                                flow_from_direction);
}

bool LiquidBlock::_isLiquidBlocking(BlockSource &region, BlockPos const &pos, BlockPos const &flow_from_pos,
                                    unsigned char flow_from_direction) const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&LiquidBlock::_isLiquidBlocking, this, region, pos, flow_from_pos,
                                       flow_from_direction);
}
