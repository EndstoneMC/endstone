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

#include "endstone/event/event.h"
#include "endstone/level.h"

namespace endstone {

class WeatherEvent : public Event {
public:
    explicit WeatherEvent(Level &level) : level_(level){};
    ~WeatherEvent() override = default;

    /**
     * Returns the Level where this event is occurring
     *
     * @return Level this event is occurring in
     */
    [[nodiscard]] Level &getLevel() const
    {
        return level_;
    }

private:
    Level &level_;
};

}  // namespace endstone
