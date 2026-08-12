// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Represents the type of animation performed by a player.
 */
enum class PlayerAnimationType {
    /**
     * The player swings their main hand.
     */
    ArmSwing,
    /**
     * The player swings their off hand.
     */
    OffArmSwing,
};

/**
 * Represents an event that is called when a player performs an animation.
 */
class PlayerAnimationEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerAnimationEvent);

    PlayerAnimationEvent(const NotNull<Player> &player, PlayerAnimationType animation_type)
        : PlayerEvent(player), animation_type_(animation_type)
    {
    }

    /**
     * Gets the type of animation performed by the player.
     *
     * @return the animation type
     */
    [[nodiscard]] PlayerAnimationType getAnimationType() const { return animation_type_; }

private:
    PlayerAnimationType animation_type_;
};

}  // namespace endstone
