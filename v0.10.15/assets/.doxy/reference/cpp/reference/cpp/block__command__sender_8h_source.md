

# File block\_command\_sender.h

[**File List**](files.md) **>** [**command**](dir_5c7b2dbfabcd1115569d1e20a260545c.md) **>** [**block\_command\_sender.h**](block__command__sender_8h.md)

[Go to the documentation of this file](block__command__sender_8h.md)


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
#include "endstone/command/command_sender.h"

namespace endstone {

class BlockCommandSender : public CommandSender {
public:
    [[nodiscard]] virtual std::unique_ptr<Block> getBlock() const = 0;
};

}  // namespace endstone
```


