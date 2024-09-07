

# File command\_sender.h

[**File List**](files.md) **>** [**command**](dir_5c7b2dbfabcd1115569d1e20a260545c.md) **>** [**command\_sender.h**](command__sender_8h.md)

[Go to the documentation of this file](command__sender_8h.md)


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

#include <string>

#include <fmt/format.h>

#include "endstone/message.h"
#include "endstone/permissions/permissible.h"

namespace endstone {

class ConsoleCommandSender;
class Server;
class Player;

class CommandSender : public Permissible {
public:
    CommandSender() = default;
    ~CommandSender() override = default;

    // Permissible
    [[nodiscard]] CommandSender *asCommandSender() const override
    {
        return const_cast<CommandSender *>(this);
    }

    [[nodiscard]] virtual ConsoleCommandSender *asConsole() const
    {
        return nullptr;
    }

    [[nodiscard]] virtual Player *asPlayer() const
    {
        return nullptr;
    }

    virtual void sendMessage(const Message &message) const = 0;

    template <typename... Args>
    void sendMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        try {
            sendMessage(fmt::format(format, std::forward<Args>(args)...));
        }
        catch (std::exception &e) {
            sendErrorMessage(e.what());
        }
    }

    virtual void sendErrorMessage(const Message &message) const = 0;

    template <typename... Args>
    void sendErrorMessage(const fmt::format_string<Args...> format, Args &&...args) const
    {
        sendErrorMessage(fmt::format(format, std::forward<Args>(args)...));
    }

    [[nodiscard]] virtual Server &getServer() const = 0;

    [[nodiscard]] virtual std::string getName() const = 0;
};
}  // namespace endstone
```


