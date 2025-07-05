#include "bedrock/world/level/block/actor/piston_block_actor.h"

#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_piston_event.h"
#include "endstone/event/block/block_piston_extend_event.h"
#include "endstone/event/block/block_piston_retract_event.h"
#include "endstone/runtime/hook.h"

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
                    auto block = endstone::core::EndstoneBlock::at(region, position_);
                    const auto face = block->getMinecraftBlock().getState<FacingID>("facing_direction");
                    event = std::make_unique<endstone::BlockPistonRetractEvent>(
                        std::move(block),
                        endstone::core::EndstoneBlockFace::getOpposite(static_cast<endstone::BlockFace>(face)));
                }
            }
            else {
                if (state_ == PistonState::Retracted && new_state_ == PistonState::Retracted) {
                    auto block = endstone::core::EndstoneBlock::at(region, position_);
                    const auto face = block->getMinecraftBlock().getState<FacingID>("facing_direction");
                    event = std::make_unique<endstone::BlockPistonExtendEvent>(std::move(block),
                                                                               static_cast<endstone::BlockFace>(face));
                }
            }
        }
    }

    if (event) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        server.getPluginManager().callEvent(*event);
        if (event->isCancelled()) {
            // BlockActor::tick(region);
            // _tryFixupStickyPistonArm(region); // TODO: implement this
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PistonBlockActor::tick, this, region);
}
