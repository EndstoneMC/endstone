// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include <utility>
#include <vector>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class Actor;

/**
 * Called when a player shears an actor.
 */
class PlayerShearActorEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerShearActorEvent);

    /**
     * Constructs a player shear actor event.
     *
     * @param player player who sheared the actor
     * @param actor actor that was sheared
     * @param hand hand used to shear the actor
     * @param item item used to shear the actor
     * @param drops items dropped when the actor is sheared
     */
    PlayerShearActorEvent(const NotNull<Player> &player, const NotNull<Actor> &actor, EquipmentSlot hand,
                           ItemStack item, std::vector<ItemStack> drops)
        : Cancellable(player), actor_(actor), hand_(hand), item_(std::move(item)), drops_(std::move(drops))
    {
    }

    /**
     * Gets the actor that was sheared.
     *
     * @return actor that was sheared
     */
    [[nodiscard]] const NotNull<Actor> &getActor() const { return actor_; }

    /**
     * Gets the hand used to shear the actor.
     *
     * @return hand used to shear the actor
     */
    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

    /**
     * Gets the item used to shear the actor.
     *
     * @return item used to shear the actor
     */
    [[nodiscard]] const ItemStack &getItem() const { return item_; }

    /**
     * Gets the items dropped when the actor is sheared.
     *
     * @return items dropped when the actor is sheared
     */
    [[nodiscard]] const std::vector<ItemStack> &getDrops() const { return drops_; }

    /**
     * Sets the items dropped when the actor is sheared.
     *
     * @param drops items to drop when the actor is sheared
     */
    void setDrops(std::vector<ItemStack> drops) { drops_ = std::move(drops); }

private:
    NotNull<Actor> actor_;
    EquipmentSlot hand_;
    ItemStack item_;
    std::vector<ItemStack> drops_;
};

}  // namespace endstone
