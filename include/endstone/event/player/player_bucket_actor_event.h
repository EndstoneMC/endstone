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

#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class Actor;

/**
 * Represents an event that is called when a player captures an actor in a bucket.
 */
class PlayerBucketActorEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerBucketActorEvent);

    PlayerBucketActorEvent(const NotNull<Player> &player, const NotNull<Actor> &actor, ItemStack original_bucket,
                           EquipmentSlot hand)
        : Cancellable(player), actor_(actor), original_bucket_(std::move(original_bucket)), hand_(hand)
    {
    }

    /**
     * Gets the actor being captured.
     *
     * @return actor being captured
     */
    [[nodiscard]] const NotNull<Actor> &getActor() const { return actor_; }

    /**
     * Gets the bucket used to capture the actor. This refers to the bucket clicked with, i.e. a water bucket.
     *
     * @return bucket used to capture the actor
     */
    [[nodiscard]] const ItemStack &getOriginalBucket() const { return original_bucket_; }

    /**
     * Gets the hand used to capture the actor.
     *
     * @return hand used to capture the actor
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

private:
    NotNull<Actor> actor_;
    ItemStack original_bucket_;
    EquipmentSlot hand_;
};

}  // namespace endstone
