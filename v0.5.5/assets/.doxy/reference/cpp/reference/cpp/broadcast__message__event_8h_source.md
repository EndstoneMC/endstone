

# File broadcast\_message\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**server**](dir_77022909323d5ad872c4820a738a5429.md) **>** [**broadcast\_message\_event.h**](broadcast__message__event_8h.md)

[Go to the documentation of this file](broadcast__message__event_8h.md)


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
#include <utility>

#include "endstone/command/command_sender.h"
#include "endstone/event/event.h"
#include "endstone/event/handler_list.h"
#include "endstone/event/server/server_event.h"

namespace endstone {

class BroadcastMessageEvent : public ServerEvent {
public:
    BroadcastMessageEvent(bool async, std::string message, std::unordered_set<const CommandSender *> recipients)
        : ServerEvent(async), message_(std::move(message)), recipients_(std::move(recipients))
    {
    }

    inline static const std::string NAME = "BroadcastMessageEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    [[nodiscard]] const std::string &getMessage() const
    {
        return message_;
    }

    void setMessage(std::string message)
    {
        message_ = std::move(message);
    }

    [[nodiscard]] const std::unordered_set<const CommandSender *> &getRecipients() const
    {
        return recipients_;
    }

private:
    std::string message_;
    std::unordered_set<const CommandSender *> recipients_;
};

}  // namespace endstone
```


