

# File actor\_teleport\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_teleport\_event.h**](actor__teleport__event_8h.md)

[Go to the documentation of this file](actor__teleport__event_8h.md)


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

#include "endstone/event/actor/actor_event.h"
#include "endstone/level/location.h"

namespace endstone {

class ActorTeleportEvent : public ActorEvent {
public:
    explicit ActorTeleportEvent(Actor &actor, Location from, Location to) : ActorEvent(actor), from_(from), to_(to) {}
    ~ActorTeleportEvent() override = default;

    inline static const std::string NAME = "ActorTeleportEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    [[nodiscard]] const Location &getFrom() const
    {
        return from_;
    }

    void setFrom(const Location &from)
    {
        from_ = from;
    }

    [[nodiscard]] const Location &getTo() const
    {
        return to_;
    }

    void setTo(const Location &to)
    {
        to_ = to;
    }

private:
    Location from_;
    Location to_;
};

}  // namespace endstone
```


