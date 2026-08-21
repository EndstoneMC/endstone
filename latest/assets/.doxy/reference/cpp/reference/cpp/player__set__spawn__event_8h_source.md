

# File player\_set\_spawn\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_set\_spawn\_event.h**](player__set__spawn__event_8h.md)

[Go to the documentation of this file](player__set__spawn__event_8h.md)


```C++
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

#include <optional>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/level/location.h"

namespace endstone {

class PlayerSetSpawnEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerSetSpawnEvent);

    enum class Cause {
        Bed,
        RespawnAnchor,
        Command,
        Plugin,
        Unknown,
    };

    PlayerSetSpawnEvent(const NotNull<Player> &player, Cause cause, std::optional<Location> location)
        : Cancellable(player), cause_(cause), location_(std::move(location))
    {
    }

    [[nodiscard]] Cause getCause() const { return cause_; }

    [[nodiscard]] const std::optional<Location> &getLocation() const { return location_; }

    void setLocation(std::optional<Location> location) { location_ = std::move(location); }

private:
    Cause cause_;
    std::optional<Location> location_;
};

}  // namespace endstone
```


