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

#include "endstone/event/weather/weather_event.h"

namespace endstone {

/**
 * Called when the thunder state in a world is changing
 */
class ThunderChangeEvent : public WeatherEvent {
public:
    ThunderChangeEvent(Level &level, bool to) : WeatherEvent(level), to_(to) {}
    ~ThunderChangeEvent() override = default;

    /**
     * Gets the state of thunder that the world is being set to
     *
     * @return true if the weather is being set to thundering, false otherwise
     */
    [[nodiscard]] bool toThunderState() const
    {
        return to_;
    }

    inline static const std::string NAME = "ThunderChangeEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

private:
    bool to_;
};

}  // namespace endstone
