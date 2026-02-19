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

#include "bedrock/world/level/weather_manager.h"

#include "endstone/core/server.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"
#include "endstone/runtime/hook.h"

void WeatherManager::updateWeather(float rain_level, int rain_time, float lightning_level, int lightning_time)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (auto *level = server.getLevel()) {
        auto is_raining = weather_manager_proxy_->getRainLevel() > 0;
        auto will_be_raining = rain_level > 0;
        if (is_raining != will_be_raining) {
            endstone::WeatherChangeEvent e(*level, will_be_raining);
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                rain_level = weather_manager_proxy_->getRainLevel();
            }
        }

        auto is_lightning = weather_manager_proxy_->getLightningLevel() > 0;
        auto will_be_lightning = lightning_level > 0;
        if (is_lightning != will_be_lightning) {
            endstone::ThunderChangeEvent e(*level, will_be_lightning);
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                lightning_level = weather_manager_proxy_->getLightningLevel();
            }
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&WeatherManager::updateWeather, this, rain_level, rain_time, lightning_level,
                                lightning_time);
}
