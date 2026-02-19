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

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/world/events/level_event_coordinator.h"
#include "bedrock/world/level/dimension_manager.h"

class IWeatherManagerProxy {
public:
    virtual ~IWeatherManagerProxy();
    virtual float getRainLevel() const = 0;
    virtual float getLightningLevel() const = 0;
    virtual int getRainTime() const = 0;
    virtual int getLightningTime() const = 0;
    virtual void setRainLevel(float) = 0;
    virtual void setLightningLevel(float) = 0;
    virtual void setRainTime(int) = 0;
    virtual void setLightningTime(int) = 0;
};

class WeatherManager {
public:
    ENDSTONE_HOOK void updateWeather(float rain_level, int rain_time, float lightning_level, int lightning_time);
    void stopWeather();

private:
    const Bedrock::NotNullNonOwnerPtr<DimensionManager> dimension_manager_;
    const Bedrock::NotNullNonOwnerPtr<LevelEventCoordinator> level_event_coordinator_;
    const Bedrock::NotNullNonOwnerPtr<LevelEventManager> level_event_manager_;
    gsl::not_null<std::unique_ptr<IWeatherManagerProxy>> weather_manager_proxy_;
    Bedrock::PubSub::Subscription on_wake_up_all_players_subscription_;
};
