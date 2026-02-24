

# File block\_cook\_event.h

[**File List**](files.md) **>** [**block**](dir_992e9ad7dc69726476903ba283e33c71.md) **>** [**block\_cook\_event.h**](block__cook__event_8h.md)

[Go to the documentation of this file](block__cook__event_8h.md)


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

#include <utility>

#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
class BlockCookEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockCookEvent);

    BlockCookEvent(std::unique_ptr<Block> block, ItemStack source, ItemStack result)
        : Cancellable(std::move(block)), source_(std::move(source)), result_(std::move(result))
    {
    }

    [[nodiscard]] const ItemStack &getSource() const { return source_; }

    [[nodiscard]] const ItemStack &getResult() const { return result_; }

    void setResult(ItemStack result) { result_ = std::move(result); }

private:
    ItemStack source_;
    ItemStack result_;
};
}  // namespace endstone
```


