// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

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
class CauldronLevelChangeEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(CauldronLevelChangeEvent);

    /**
     * Describes what caused the cauldron to change.
     */
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
        /** An actor was extinguished. */
        Extinguish,
        /** The cauldron evaporated. Bedrock never evaporates a cauldron, so this is never reported. */
        Evaporate,
        /** The cauldron was filled by a natural fluid source, e.g. rain or dripstone. */
        NaturalFill,
        /** The cause is unknown. */
        Unknown,
    };

    CauldronLevelChangeEvent(const NotNull<Block> &block, const Nullable<Actor> &actor, ChangeReason reason,
                             const NotNull<BlockState> &new_state)
        : Cancellable(block), actor_(actor), reason_(reason), new_state_(new_state)
    {
    }

    /**
     * Gets the actor which did this.
     *
     * Bedrock does not report which actor changed a cauldron, so this is always nullptr.
     *
     * @return the actor which did this, or nullptr if there is none
     */
    [[nodiscard]] const Nullable<Actor> &getActor() const { return actor_; }

    /**
     * Gets the reason for the change.
     *
     * Bedrock does not report why a cauldron changed, so this is always ChangeReason::Unknown.
     *
     * @return the reason for the change
     */
    [[nodiscard]] ChangeReason getReason() const { return reason_; }

    /**
     * Gets the state the cauldron will take.
     *
     * Modifying the returned state changes what the cauldron becomes.
     *
     * @return the new state of the cauldron
     */
    [[nodiscard]] const NotNull<BlockState> &getNewState() const { return new_state_; }

private:
    Nullable<Actor> actor_;
    ChangeReason reason_;
    NotNull<BlockState> new_state_;
};

}  // namespace endstone
