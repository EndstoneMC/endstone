

# File player\_death\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**player\_death\_event.h**](player__death__event_8h.md)

[Go to the documentation of this file](player__death__event_8h.md)


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

#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/player/player_event.h"

namespace endstone {
class PlayerDeathEvent : public ActorDeathEvent {
public:
    ENDSTONE_EVENT(PlayerDeathEvent);
    explicit PlayerDeathEvent(Player &player, std::unique_ptr<DamageSource> damage_source,
                              std::optional<Message> death_message)
        : ActorDeathEvent(player, std::move(damage_source)), death_message_(std::move(death_message))
    {
    }

    [[nodiscard]] Player &getPlayer() const { return static_cast<Player &>(getActor()); }

    [[nodiscard]] std::optional<Message> getDeathMessage() const { return death_message_; }

    void setDeathMessage(std::optional<Message> death_message) { death_message_ = std::move(death_message); }

private:
    std::optional<Message> death_message_;

    // TODO(event): new exp, new level, new total exp, keep level, keep inventory
};
}  // namespace endstone
```


