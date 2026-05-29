

# File block\_break\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**block\_break\_event.h**](block__break__event_8h.md)

[Go to the documentation of this file](block__break__event_8h.md)


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

#include "endstone/event/block/block_event.h"
#include "endstone/player.h"

namespace endstone {

class BlockBreakEvent : public BlockEvent {
public:
    explicit BlockBreakEvent(Block &block, Player &player) : BlockEvent(block), player_(player) {}
    ~BlockBreakEvent() override = default;

    inline static const std::string NAME = "BlockBreakEvent";
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

private:
    Player &player_;
};

}  // namespace endstone
```


