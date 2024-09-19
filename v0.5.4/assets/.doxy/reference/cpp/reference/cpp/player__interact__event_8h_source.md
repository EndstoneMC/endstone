

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

#include "endstone/event/player/player_event.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class PlayerInteractEvent : public PlayerEvent {
public:
    PlayerInteractEvent(Player &player, std::unique_ptr<ItemStack> item, std::unique_ptr<Block> block_clicked,
                        BlockFace block_face, const Vector<float> &clicked_position)
        : PlayerEvent(player), item_(std::move(item)), block_clicked_(std::move(block_clicked)),
          block_face_(block_face), clicked_position_(clicked_position)
    {
    }
    ~PlayerInteractEvent() override = default;

    inline static const std::string NAME = "PlayerInteractEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    [[nodiscard]] bool hasItem() const
    {
        return item_ != nullptr;
    }

    [[nodiscard]] ItemStack *getItem() const
    {
        return item_.get();
    }

    [[nodiscard]] bool hasBlock() const
    {
        return block_clicked_ != nullptr;
    }

    [[nodiscard]] Block *getBlock() const
    {
        return block_clicked_.get();
    }

    [[nodiscard]] BlockFace getBlockFace() const
    {
        return block_face_;
    }

    [[nodiscard]] Vector<float> getClickedPosition() const
    {
        return clicked_position_;
    }

private:
    std::unique_ptr<ItemStack> item_;
    std::unique_ptr<Block> block_clicked_;
    BlockFace block_face_;
    Vector<float> clicked_position_;
};

}  // namespace endstone
```


