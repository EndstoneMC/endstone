

# File weather\_change\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**weather**](dir_7fcf87d2683114df01ea446fea23c187.md) **>** [**weather\_change\_event.h**](weather__change__event_8h.md)

[Go to the documentation of this file](weather__change__event_8h.md)


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

#include "endstone/event/cancellable.h"
#include "endstone/event/weather/weather_event.h"

namespace endstone {

class WeatherChangeEvent : public Cancellable<WeatherEvent> {
public:
    WeatherChangeEvent(Level &level, const bool to) : Cancellable(level), to_(to) {}
    ~WeatherChangeEvent() override = default;

    [[nodiscard]] bool toWeatherState() const
    {
        return to_;
    }

    inline static const std::string NAME = "WeatherChangeEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

private:
    bool to_;
};

}  // namespace endstone
```


