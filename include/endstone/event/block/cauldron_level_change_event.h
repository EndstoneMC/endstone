#pragma once

#include <memory>
#include <utility>

#include "endstone/actor/actor.h"
#include "endstone/block/block_state.h"
#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * Called when a cauldron's level or contents change.
 *
 * If this event is cancelled, the cauldron will not change.
 */
class CauldronLevelChangeEvent final : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(CauldronLevelChangeEvent);

    /** Describes what caused the cauldron to change. */
    enum class ChangeReason {
        /** A player filled a bucket from the cauldron. */
        BucketFill,
        /** A player emptied a bucket into the cauldron. */
        BucketEmpty,
        /** A player filled a bottle from the cauldron. */
        BottleFill,
        /** A player emptied a bottle into the cauldron. */
        BottleEmpty,
        /** A player washed a banner. */
        BannerWash,
        /** A player washed armor. */
        ArmorWash,
        /** A player washed a shulker box. */
        ShulkerWash,
        /** An entity was extinguished. */
        Extinguish,
        /** The cauldron evaporated. */
        Evaporate,
        /** The cauldron was filled by rain or dripstone. */
        NaturalFill,
        /** The cause is unknown. */
        Unknown,
    };

    CauldronLevelChangeEvent(std::unique_ptr<Block> block, Nullable<Actor> entity, ChangeReason reason,
                             std::unique_ptr<BlockState> new_state)
        : Cancellable(std::move(block)), entity_(std::move(entity)), reason_(reason), new_state_(std::move(new_state))
    {
    }

    /**
     * Gets the entity responsible for the change.
     *
     * @return the responsible entity, or nullptr if there is none
     */
    [[nodiscard]] const Nullable<Actor> &getEntity() const { return entity_; }

    /**
     * Gets the reason for the change.
     *
     * @return the change reason
     */
    [[nodiscard]] ChangeReason getReason() const { return reason_; }

    /**
     * Gets the state that will replace the cauldron.
     *
     * @return the new block state
     */
    [[nodiscard]] BlockState &getNewState() const { return *new_state_; }

private:
    Nullable<Actor> entity_;
    ChangeReason reason_;
    std::unique_ptr<BlockState> new_state_;
};

}  // namespace endstone
