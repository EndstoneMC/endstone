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

#include "bedrock/event/gameplay_handler.h"

#include "bedrock/event/coordinator_result.h"
#include "bedrock/event/level_event.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"

using endstone::detail::EndstoneServer;
using endstone::detail::Singleton;

GameplayHandlerResult<CoordinatorResult> ScriptLevelGameplayHandler::handleEvent(LevelWeatherChangedEvent &event)
{
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

    GameplayHandlerResult<CoordinatorResult> result;
#ifdef _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_RVO_NAME(&ScriptLevelGameplayHandler::handleEvent, __FUNCDNAME__, result, this, event);
#else
    result = ENDSTONE_HOOK_CALL_ORIGINAL_NAME(
        &ScriptLevelGameplayHandler::handleEvent,
        "_ZN26ScriptLevelGameplayHandler11handleEventER24LevelWeatherChangedEvent", this, event);
#endif
    return result;
}
