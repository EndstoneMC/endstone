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

#include "endstone/detail/gameplay_handler_adapter.h"

#include "bedrock/event/coordinator_result.h"
#include "bedrock/event/event_result.h"
#include "bedrock/event/level_event.h"
#include "bedrock/gameplay_handler.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"

using endstone::detail::EndstoneServer;
using endstone::detail::Singleton;

namespace endstone::detail {

EndstoneGameplayHandlerAdapter::EndstoneGameplayHandlerAdapter(ScriptLevelGameplayHandler &script_handler)
    : level_handler_(script_handler)
{
    level_handler_.hook<3, 4>(&EndstoneGameplayHandlerAdapter::onLevelWeatherChanged);
}

GameplayHandlerResult<CoordinatorResult> EndstoneGameplayHandlerAdapter::onLevelWeatherChanged(
    LevelWeatherChangedEvent &event)
{
    auto result = level_handler_.callOriginal(&EndstoneGameplayHandlerAdapter::onLevelWeatherChanged, event);
    if (result.value == CoordinatorResult::Deny || result.result == EventResult::Deny) {
        return result;
    }

    auto &server = Singleton<EndstoneServer>::getInstance();
    if (event.from_rain != event.to_rain) {
        endstone::WeatherChangeEvent e(event.to_rain);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.to_rain = event.from_rain;
        }
    }

    if (event.from_lightning != event.to_lightning) {
        endstone::ThunderChangeEvent e(event.to_lightning);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.to_lightning = event.from_lightning;
        }
    }

    return result;
}

}  // namespace endstone::detail
