

# File actor\_knockback\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_knockback\_event.h**](actor__knockback__event_8h.md)

[Go to the documentation of this file](actor__knockback__event_8h.md)


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

#include "endstone/actor/mob.h"
#include "endstone/event/actor/actor_event.h"

namespace endstone {

class ActorKnockbackEvent : public ActorEvent {
public:
    explicit ActorKnockbackEvent(Mob &mob, Actor *source, Vector<float> knockback)
        : ActorEvent(mob), mob_(mob), source_(source), knockback_(knockback)
    {
    }
    ~ActorKnockbackEvent() override = default;

    inline static const std::string NAME = "ActorKnockbackEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    [[nodiscard]] Mob &getActor() const
    {
        return mob_;
    }

    [[nodiscard]] Actor *getSource() const
    {
        return source_;
    }

    [[nodiscard]] Vector<float> getKnockback() const
    {
        return knockback_;
    }

    void setKnockback(Vector<float> knockback)
    {
        knockback_ = knockback;
    }

private:
    Mob &mob_;
    Actor *source_;
    Vector<float> raw_knockback_;
    Vector<float> knockback_;
};

}  // namespace endstone
```


