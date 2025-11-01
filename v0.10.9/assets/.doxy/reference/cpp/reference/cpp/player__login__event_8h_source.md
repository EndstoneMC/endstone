

# File player\_login\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_login\_event.h**](player__login__event_8h.md)

[Go to the documentation of this file](player__login__event_8h.md)


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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

class PlayerLoginEvent : public Cancellable<PlayerEvent> {
public:
    explicit PlayerLoginEvent(Player &player, std::string message = "")
        : Cancellable(player), message_(std::move(message))
    {
    }
    ~PlayerLoginEvent() override = default;

    inline static const std::string NAME = "PlayerLoginEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] const std::string &getKickMessage() const
    {
        return message_;
    }

    void setKickMessage(const std::string &message)
    {
        message_ = message;
    }

private:
    std::string message_;
};

}  // namespace endstone
```


