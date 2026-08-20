

# File actor\_effect\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_effect\_event.h**](actor__effect__event_8h.md)

[Go to the documentation of this file](actor__effect__event_8h.md)


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

#include <utility>

#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/potion/effect.h"

namespace endstone {

class ActorEffectEvent : public Cancellable<ActorEvent<Mob>> {
public:
    enum class Action {
        Added,
    };

    ENDSTONE_EVENT(ActorEffectEvent);

    explicit ActorEffectEvent(const NotNull<Mob> &mob, Action action, Effect effect)
        : Cancellable(mob), action_(action), effect_(std::move(effect))
    {
    }

    [[nodiscard]] Action getAction() const { return action_; }

    [[nodiscard]] const Effect &getEffect() const { return effect_; }

    void setEffect(Effect effect) { effect_ = std::move(effect); }

private:
    Action action_;
    Effect effect_;
};

}  // namespace endstone
```


