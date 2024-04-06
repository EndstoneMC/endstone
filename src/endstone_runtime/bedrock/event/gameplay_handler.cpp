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

#include <entt/entt.hpp>

#include "endstone/detail/server.h"
#include "endstone/detail/virtual_table.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"

using endstone::detail::EndstoneServer;
using endstone::detail::VirtualTable;

namespace endstone::detail {
template <>
void VirtualTableHook<ScriptLevelGameplayHandler>::init()
{
    hook<_WIN32_LINUX_(3, 4), LevelWeatherChangedEvent &>(&EndstoneLevelGameplayHandler::handleEvent);
}
}  // namespace endstone::detail

GameplayHandlerResult<CoordinatorResult> EndstoneLevelGameplayHandler::handleEvent(LevelWeatherChangedEvent &event)
{
    auto &vtable = entt::locator<VirtualTable<ScriptLevelGameplayHandler>>::value();

#ifdef _WIN32
    GameplayHandlerResult<CoordinatorResult> result;
    vtable.call<3, GameplayHandlerResult<CoordinatorResult>>(this, &result, event);
#else
    auto result = vtable.call<4, GameplayHandlerResult<CoordinatorResult>>(this, event);
#endif

    if (result.value == CoordinatorResult::Deny || result.result == EventResult::Deny) {
        return result;
    }

    auto &server = entt::locator<EndstoneServer>::value();

    if (event.from_rain != event.to_rain) {
        server.getLogger().debug("WeatherChangeEvent");
        endstone::WeatherChangeEvent e(event.to_rain);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.to_rain = event.from_rain;
        }
    }

    if (event.from_lightning != event.to_lightning) {
        server.getLogger().debug("ThunderChangeEvent");
        endstone::ThunderChangeEvent e(event.to_lightning);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.to_lightning = event.from_lightning;
        }
    }

    return result;
}
