

# File player\_level\_change\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_level\_change\_event.h**](player__level__change__event_8h.md)

[Go to the documentation of this file](player__level__change__event_8h.md)


```C++
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

class PlayerLevelChangeEvent final : public PlayerEvent {
public:
    ENDSTONE_EVENT(PlayerLevelChangeEvent);

    PlayerLevelChangeEvent(const NotNull<Player> &player, int old_level, int new_level)
        : PlayerEvent(player), old_level_(old_level), new_level_(new_level)
    {
    }

    [[nodiscard]] int getOldLevel() const { return old_level_; }

    [[nodiscard]] int getNewLevel() const { return new_level_; }

private:
    int old_level_;
    int new_level_;
};

}  // namespace endstone
```


