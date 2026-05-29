

# File actor\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_event.h**](actor__event_8h.md)

[Go to the documentation of this file](actor__event_8h.md)


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

#include "endstone/actor/actor.h"
#include "endstone/event/event.h"

namespace endstone {

class ActorEvent : public Event {
public:
    explicit ActorEvent(Actor &actor) : actor_(actor){};
    ~ActorEvent() override = default;

    [[nodiscard]] Actor &getActor() const
    {
        return actor_;
    }

    // TODO(event): add getActorType method

private:
    Actor &actor_;
};

}  // namespace endstone
```


