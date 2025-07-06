#include "bedrock/world/level/block/actor/piston_block_actor.h"

#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/server.h"
#include "endstone/event/block/block_piston_event.h"
#include "endstone/event/block/block_piston_extend_event.h"
#include "endstone/event/block/block_piston_retract_event.h"
#include "endstone/runtime/hook.h"

enum class PistonStateEx : char {
    // PistonState uses 0-3
    ExpandingCancelled = 4,   // Endstone
    RetractingCancelled = 5,  // Endstone
};

void PistonBlockActor::tick(BlockSource &region)
{
    bool extending = false;
    bool retracting = false;

    auto &dimension = region.getDimension();
    if (dimension.isRedstoneTick()) {
        auto &circuit = dimension.getCircuitSystem();
        const auto strength = circuit.getStrength(position_);
        if (strength != CircuitSystem::NOT_DEFINED_STRENGTH) {
            if (strength <= 0) {
                if (state_ == PistonState::Expanded && new_state_ == PistonState::Expanded) {
                    retracting = true;
                }
                else if (state_ == PistonState::Retracted &&
                         static_cast<PistonStateEx>(new_state_) == PistonStateEx::ExpandingCancelled) {
                    // If there’s no redstone signal, and we had previously cancelled an expansion,
                    // clear the sentinel and revert to the normal Retracted state so future ticks see a valid state.
                    // See notes below.
                    new_state_ = PistonState::Retracted;
                }
            }
            else {
                if (state_ == PistonState::Retracted && new_state_ == PistonState::Retracted) {
                    extending = true;
                }
                else if (state_ == PistonState::Expanded &&
                         static_cast<PistonStateEx>(new_state_) == PistonStateEx::RetractingCancelled) {
                    new_state_ = PistonState::Expanded;
                }
            }
        }
    }

    if (extending || retracting) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto block = endstone::core::EndstoneBlock::at(region, position_);
        auto face = endstone::BlockFace::Down;
        switch (block->getMinecraftBlock().getState<FacingID>(VanillaStateIds::FacingDirection)) {
        case 1:
            face = endstone::BlockFace::Up;
            break;
        case 2:
            face = endstone::BlockFace::South;
            break;
        case 3:
            face = endstone::BlockFace::North;
            break;
        case 4:
            face = endstone::BlockFace::East;
            break;
        case 5:
            face = endstone::BlockFace::West;
            break;
        default:
            break;
        }

        if (extending) {
            endstone::BlockPistonExtendEvent e(std::move(block), face);
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                // We mark the new state with a value that the original function does not recognise.
                // The vanilla logic sees an unknown state and simply does nothing (i.e., no block‐moving).
                // Yes, this can be pretty hacky, I know.
                new_state_ = static_cast<PistonState>(PistonStateEx::ExpandingCancelled);
            }
        }
        else {
            endstone::BlockPistonRetractEvent e(std::move(block), endstone::core::EndstoneBlockFace::getOpposite(face));
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                new_state_ = static_cast<PistonState>(PistonStateEx::RetractingCancelled);
            }
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PistonBlockActor::tick, this, region);
}
