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
 * Represents an event that is called when a player captures an entity with a bucket.
 */
class PlayerBucketEntityEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerBucketEntityEvent);

    PlayerBucketEntityEvent(Player &player, Actor &entity, ItemStack entity_bucket, EquipmentSlot hand,
                            ItemStack original_bucket)
        : Cancellable(player),
          entity_(entity),
          entity_bucket_(std::move(entity_bucket)),
          hand_(hand),
          original_bucket_(std::move(original_bucket))
    {
    }

    /**
     * Gets the entity being captured.
     *
     * @return entity being captured
     */
    [[nodiscard]] Actor &getEntity() const { return entity_; }

    /**
     * Gets the bucket item that will contain the captured entity.
     *
     * @return bucket item containing the captured entity
     */
    [[nodiscard]] const ItemStack &getEntityBucket() const { return entity_bucket_; }

    /**
     * Sets the bucket item that will contain the captured entity.
     *
     * @param entity_bucket new entity bucket
     */
    void setEntityBucket(ItemStack entity_bucket) { entity_bucket_ = std::move(entity_bucket); }

    /**
     * Gets the hand used to capture the entity.
     *
     * @return hand used to capture the entity
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

    /**
     * Gets the bucket used to capture the entity.
     *
     * @return bucket used to capture the entity
     */
    [[nodiscard]] const ItemStack &getOriginalBucket() const { return original_bucket_; }

private:
    Actor &entity_;
    ItemStack entity_bucket_;
    EquipmentSlot hand_;
    ItemStack original_bucket_;
};

}  // namespace endstone
