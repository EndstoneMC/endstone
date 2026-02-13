#include "bedrock/world/level/block/leaves_block.h"

#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/block/leaves_decay_event.h"
#include "endstone/runtime/hook.h"

#ifdef _WIN32
void LeavesBlock::_die(::BlockSource &region, ::BlockPos const &pos) const
#else
void LeavesBlock::_die(::BlockSource &region, ::BlockPos const &pos)
#endif
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::LeavesDecayEvent event(endstone::core::EndstoneBlock::at(region, pos));
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }
#ifdef _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL(&LeavesBlock::_die, this, region, pos);
#else
    ENDSTONE_HOOK_CALL_ORIGINAL(&LeavesBlock::_die, region, pos);
#endif
}
