#include "bedrock/world/level/block/leaves_block.h"

#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/block/leaves_decay_event.h"
#include "endstone/runtime/hook.h"

void LeavesBlock::_die(::BlockSource &region, ::BlockPos const &pos) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::LeavesDecayEvent event(endstone::core::EndstoneBlock::at(region, pos));
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&LeavesBlock::_die, this, region, pos);
}
