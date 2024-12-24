

# File block\_place\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**block\_place\_event.h**](block__place__event_8h.md)

[Go to the documentation of this file](block__place__event_8h.md)


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

#include "endstone/block/block_state.h"
#include "endstone/event/block/block_event.h"
#include "endstone/player.h"

namespace endstone {

class BlockPlaceEvent : public BlockEvent {
public:
    explicit BlockPlaceEvent(std::unique_ptr<BlockState> placed_block, Block &replaced_block, Block &placed_against,
                             Player &player)
        : BlockEvent(replaced_block), placed_block_(std::move(placed_block)), placed_against_(placed_against),
          player_(player)
    {
    }
    ~BlockPlaceEvent() override = default;

    inline static const std::string NAME = "BlockPlaceEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    [[nodiscard]] Player &getPlayer() const
    {
        return player_;
    }

    [[nodiscard]] BlockState &getBlockPlacedState() const
    {
        return *placed_block_;
    }

    [[nodiscard]] Block &getBlockReplaced() const
    {
        return getBlock();
    }

    [[nodiscard]] Block &getBlockAgainst() const
    {
        return placed_against_;
    }

private:
    std::unique_ptr<BlockState> placed_block_;
    Block &placed_against_;
    Player &player_;
    // TODO(event): add ItemStack item
    // TODO(event): add BlockState placedBlockState
};

}  // namespace endstone
```


