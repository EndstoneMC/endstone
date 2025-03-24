

# File block\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**block\_event.h**](block__event_8h.md)

[Go to the documentation of this file](block__event_8h.md)


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
#include "endstone/event/event.h"

namespace endstone {

class BlockEvent : public Event {
public:
    explicit BlockEvent(Block &block) : block_(block){};
    ~BlockEvent() override = default;

    [[nodiscard]] Block &getBlock() const
    {
        return block_;
    }

private:
    Block &block_;
};

}  // namespace endstone
```


