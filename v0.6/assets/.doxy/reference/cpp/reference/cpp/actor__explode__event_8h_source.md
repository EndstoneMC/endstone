

# File actor\_explode\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_explode\_event.h**](actor__explode__event_8h.md)

[Go to the documentation of this file](actor__explode__event_8h.md)


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

#include "endstone/block/block.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/level/location.h"

namespace endstone {

class ActorExplodeEvent : public Cancellable<ActorEvent<Actor>> {
    using BlockList = std::vector<std::shared_ptr<Block>>;

public:
    explicit ActorExplodeEvent(Actor &actor, Location location, BlockList blocks)
        : Cancellable(actor), location_(location), blocks_(std::move(blocks))
    {
    }
    ~ActorExplodeEvent() override = default;

    inline static const std::string NAME = "ActorExplodeEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] const Location &getLocation() const
    {
        return location_;
    }

    [[nodiscard]] const BlockList &getBlockList() const
    {
        return blocks_;
    }

    [[nodiscard]] BlockList &getBlockList()
    {
        return blocks_;
    }

private:
    Location location_;
    BlockList blocks_;
};

}  // namespace endstone
```


