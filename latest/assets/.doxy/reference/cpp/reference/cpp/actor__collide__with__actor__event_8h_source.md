

# File actor\_collide\_with\_actor\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_collide\_with\_actor\_event.h**](actor__collide__with__actor__event_8h.md)

[Go to the documentation of this file](actor__collide__with__actor__event_8h.md)


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

#include <vector>

#include "endstone/actor/actor.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/event.h"
#include "endstone/util/pointers.h"

namespace endstone {

class ActorCollideWithActorEvent final : public Cancellable<Event> {
public:
    ENDSTONE_EVENT(ActorCollideWithActorEvent);

    ActorCollideWithActorEvent(const NotNull<Actor> &actor1, const NotNull<Actor> &actor2) : actors_{actor1, actor2} {}

    [[nodiscard]] const std::vector<NotNull<Actor>> &getActors() const { return actors_; }

private:
    std::vector<NotNull<Actor>> actors_;
};

}  // namespace endstone
```


