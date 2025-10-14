

# File block\_piston\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**block\_piston\_event.h**](block__piston__event_8h.md)

[Go to the documentation of this file](block__piston__event_8h.md)


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
#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

class BlockPistonEvent : public Cancellable<BlockEvent> {
public:
    explicit BlockPistonEvent(std::unique_ptr<Block> block, BlockFace direction)
        : Cancellable(std::move(block)), direction_(direction)
    {
    }

    [[nodiscard]] BlockFace getDirection() const
    {
        return direction_;
    }

private:
    BlockFace direction_;
};

}  // namespace endstone
```


