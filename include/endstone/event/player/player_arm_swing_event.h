// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#pragma once

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * Called when a player swings their arm.
 */
class PlayerArmSwingEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerArmSwingEvent);

    explicit PlayerArmSwingEvent(const NotNull<Player> &player) : PlayerEvent(player) {}
};

}  // namespace endstone
