#include "bedrock/world/level/block/liquid_block.h"

#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_from_to_event.h"
#include "endstone/runtime/hook.h"

void LiquidBlock::_trySpreadTo(BlockSource &region, BlockPos const &pos, int neighbor, BlockPos const &flow_from_pos,
                               FacingID flow_from_direction) const
{
    if (_canSpreadTo(region, pos, flow_from_pos, flow_from_direction) && !region.isInstaticking(pos)) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
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
