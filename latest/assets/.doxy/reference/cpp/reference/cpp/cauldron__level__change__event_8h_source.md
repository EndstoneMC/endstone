

# File cauldron\_level\_change\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**cauldron\_level\_change\_event.h**](cauldron__level__change__event_8h.md)

[Go to the documentation of this file](cauldron__level__change__event_8h.md)


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
#include "endstone/block/block_state.h"
#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/util/pointers.h"

namespace endstone {

class CauldronLevelChangeEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(CauldronLevelChangeEvent);

    enum class ChangeReason {
        BucketFill,
        BucketEmpty,
        BottleFill,
        BottleEmpty,
        BannerWash,
        ArmorWash,
        ShulkerWash,
        Extinguish,
        Evaporate,
        NaturalFill,
        Unknown,
    };

    CauldronLevelChangeEvent(const NotNull<Block> &block, const Nullable<Actor> &actor, ChangeReason reason,
                             const NotNull<BlockState> &new_state)
        : Cancellable(block), actor_(actor), reason_(reason), new_state_(new_state)
    {
    }

    [[nodiscard]] const Nullable<Actor> &getActor() const { return actor_; }

    [[nodiscard]] ChangeReason getReason() const { return reason_; }

    [[nodiscard]] const NotNull<BlockState> &getNewState() const { return new_state_; }

private:
    Nullable<Actor> actor_;
    ChangeReason reason_;
    NotNull<BlockState> new_state_;
};

}  // namespace endstone
```


