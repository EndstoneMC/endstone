// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "endstone/event/player/player_event.h"
#include "endstone/util/vector.h"

namespace endstone {

/**
 * Represents an event that is called when the velocity of a player changes.
 */
class PlayerVelocityEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerVelocityEvent);

    explicit PlayerVelocityEvent(Player &player, Vector velocity) : PlayerEvent(player), velocity_(velocity) {}

    /**
     * Gets the velocity vector that will be sent to the player.
     *
     * @note This method returns a copy; changes must be applied via setVelocity().
     *
     * @return the velocity vector
     */
    [[nodiscard]] Vector getVelocity() const { return velocity_; }

    /**
     * Sets the velocity vector that will be sent to the player.
     *
     * @param velocity The velocity vector.
     */
    void setVelocity(Vector velocity) { velocity_ = velocity; }

private:
    Vector velocity_;
};

}  // namespace endstone
