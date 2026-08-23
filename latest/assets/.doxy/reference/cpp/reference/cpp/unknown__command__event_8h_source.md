

# File unknown\_command\_event.h

[**File List**](files.md) **>** [**command**](dir_05a1f6e1b04260aa3cefb17cb1753190.md) **>** [**unknown\_command\_event.h**](unknown__command__event_8h.md)

[Go to the documentation of this file](unknown__command__event_8h.md)


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

#include <optional>
#include <string>
#include <utility>

#include "endstone/command/command_sender.h"
#include "endstone/event/event.h"
#include "endstone/message.h"

namespace endstone {

class UnknownCommandEvent final : public Event {
public:
    ENDSTONE_EVENT(UnknownCommandEvent);

    UnknownCommandEvent(const NotNull<CommandSender> &sender, std::string command_line, std::optional<Message> message)
        : sender_(sender), command_line_(std::move(command_line)), message_(std::move(message))
    {
    }

    [[nodiscard]] const NotNull<CommandSender> &getSender() const { return sender_; }

    [[nodiscard]] std::string getCommandLine() const { return command_line_; }

    [[nodiscard]] std::optional<Message> getMessage() const { return message_; }

    void setMessage(std::optional<Message> message) { message_ = std::move(message); }

private:
    NotNull<CommandSender> sender_;
    std::string command_line_;
    std::optional<Message> message_;
};

}  // namespace endstone
```


