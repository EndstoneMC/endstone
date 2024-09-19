

# File command\_executor.h

[**File List**](files.md) **>** [**command**](dir_5c7b2dbfabcd1115569d1e20a260545c.md) **>** [**command\_executor.h**](command__executor_8h.md)

[Go to the documentation of this file](command__executor_8h.md)


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

#include <map>
#include <string>

#include "endstone/command/command.h"
#include "endstone/command/command_sender.h"

namespace endstone {
class CommandExecutor {
public:
    virtual ~CommandExecutor() = default;

    virtual bool onCommand(CommandSender &sender, const Command &command, const std::vector<std::string> &args)
    {
        return false;
    }
};
}  // namespace endstone
```


