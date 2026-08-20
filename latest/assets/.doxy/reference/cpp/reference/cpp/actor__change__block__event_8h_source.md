

# File actor\_change\_block\_event.h

[**File List**](files.md) **>** [**actor**](dir_621c26b5fd4198aba66e7e31570ce44a.md) **>** [**actor\_change\_block\_event.h**](actor__change__block__event_8h.md)

[Go to the documentation of this file](actor__change__block__event_8h.md)


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

#include <memory>
#include <utility>

#include "endstone/block/block.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

class ActorChangeBlockEvent : public Cancellable<ActorEvent<Actor>> {
public:
    ENDSTONE_EVENT(ActorChangeBlockEvent);

    explicit ActorChangeBlockEvent(const NotNull<Actor> &actor, const NotNull<Block> &block)
        : Cancellable(actor), block_(block)
    {
    }

    [[nodiscard]] const NotNull<Block> &getBlock() const { return block_; }

private:
    NotNull<Block> block_;
};

}  // namespace endstone
```


