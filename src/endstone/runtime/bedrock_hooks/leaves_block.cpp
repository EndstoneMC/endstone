#include "bedrock/world/level/block/leaves_block.h"

#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/server.h"
#include "endstone/event/block/leaves_decay_event.h"
#include "endstone/runtime/hook.h"

void LeavesBlock::_die(::BlockSource &region, ::BlockPos const &pos) const
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto block = endstone::core::EndstoneBlock::at(region, pos);
    endstone::LeavesDecayEvent event(*block);
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&LeavesBlock::_die, this, region, pos);
}
