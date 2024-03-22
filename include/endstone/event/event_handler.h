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

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "endstone/event/event_priority.h"

namespace endstone {

class EventHandler {
public:
    EventHandler(std::function<void(Event &)> executor, EventPriority priority, Plugin &plugin, bool ignore_cancelled)
        : executor_(executor), priority_(priority), plugin_(plugin), ignore_cancelled_(ignore_cancelled)
    {
    }

    [[nodiscard]] EventPriority getPriority() const
    {
        return priority_;
    }

    [[nodiscard]] Plugin &getPlugin() const
    {
        return plugin_;
    }

    [[nodiscard]] bool isIgnoreCancelled() const
    {
        return ignore_cancelled_;
    }

    void callEvent(Event &event)
    {
        if (event.isCancellable() && event.isCancelled() && isIgnoreCancelled()) {
            return;
        }
        executor_(event);
    };

private:
    std::function<void(Event &)> executor_;
    EventPriority priority_;
    Plugin &plugin_;
    bool ignore_cancelled_;
};

}  // namespace endstone
