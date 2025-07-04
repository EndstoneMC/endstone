#include "bedrock/world/level/block/actor/piston_block_actor.h"

#include <endstone/core/server.h>
#include <endstone/event/block/block_piston_event.h>
#include <endstone/runtime/hook.h>

#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension.h"

void PistonBlockActor::tick(BlockSource &region)
{
    std::unique_ptr<endstone::BlockPistonEvent> event;
    auto &dimension = region.getDimension();
    if (dimension.isRedstoneTick()) {
        auto &circuit = dimension.getCircuitSystem();
        int strength = circuit.getStrength(position_);
        if (strength != -1) {
            if (strength <= 0) {
                if (state_ == PistonState::Expanded && new_state_ == PistonState::Expanded) {
                    // TODO: call BlockPistonRetractEvent
                    printf("BlockPistonRetractEvent\n");
                }
            }
            else {
                if (state_ == PistonState::Retracted && new_state_ == PistonState::Retracted) {
                    // TODO: call BlockPistonExtendEvent
                    printf("BlockPistonExtendEvent\n");
                }
            }
        }
    }

    if (event) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        server.getPluginManager().callEvent(*event);
        if (event->isCancelled()) {
            BlockActor::tick(region);
            _tryFixupStickyPistonArm(region);
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PistonBlockActor::tick, this, region);
}
