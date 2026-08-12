// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Represents an event that is called when a player's level changes.
 */
class PlayerLevelChangeEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerLevelChangeEvent);

    PlayerLevelChangeEvent(const NotNull<Player> &player, int old_level, int new_level)
        : PlayerEvent(player), old_level_(old_level), new_level_(new_level)
    {
    }

    /**
     * Gets the player's level before the change.
     *
     * @return the player's previous level
     */
    [[nodiscard]] int getOldLevel() const { return old_level_; }

    /**
     * Gets the player's level after the change.
     *
     * @return the player's new level
     */
    [[nodiscard]] int getNewLevel() const { return new_level_; }

private:
    int old_level_;
    int new_level_;
};

}  // namespace endstone
