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
#include <vector>

#include "endstone/event/event_listener.h"
#include "endstone/event/event_priority.h"

namespace endstone {

template <typename Event>
class HandlerList {
public:
    HandlerList() = default;

    void callEvent(Event &event)
    {
        for (auto &[priority, listeners] : listeners_) {
            for (auto &listener : listeners) {
                listener.callEvent(event);
            }
        }
    }

private:
    std::map<EventPriority, std::vector<EventListener<Event>>, std::greater<>> listeners_;
};

}  // namespace endstone
