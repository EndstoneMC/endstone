

# File player\_interact\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_interact\_event.h**](player__interact__event_8h.md)

[Go to the documentation of this file](player__interact__event_8h.md)


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

#include "endstone/block/block_face.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"

namespace endstone {

class PlayerInteractEvent : public Cancellable<PlayerEvent> {
public:
    enum class Action {
        LeftClickBlock,
        RightClickBlock,
        LeftClickAir,
        RightClickAir,
    };

    ENDSTONE_EVENT(PlayerInteractEvent);

    PlayerInteractEvent(Player &player, Action action, ItemStack *item, Block *block_clicked, BlockFace block_face,
                        const std::optional<Vector<float>> &clicked_position)
        : Cancellable(player), action_(action), item_(item), block_clicked_(block_clicked), block_face_(block_face),
          clicked_position_(clicked_position)
    {
    }

    [[nodiscard]] Action getAction() const
    {
        return action_;
    }

    [[nodiscard]] bool hasItem() const
    {
        return item_ != nullptr;
    }

    [[nodiscard]] ItemStack *getItem() const
    {
        return item_;
    }

    [[nodiscard]] bool hasBlock() const
    {
        return block_clicked_ != nullptr;
    }

    [[nodiscard]] Block *getBlock() const
    {
        return block_clicked_;
    }

    [[nodiscard]] BlockFace getBlockFace() const
    {
        return block_face_;
    }

    [[nodiscard]] std::optional<Vector<float>> getClickedPosition() const
    {
        return clicked_position_;
    }

private:
    ItemStack *item_;
    Action action_;
    Block *block_clicked_;
    BlockFace block_face_;
    std::optional<Vector<float>> clicked_position_;
};

}  // namespace endstone
```


