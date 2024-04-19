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

#include "bedrock/world/level/event/gameplay_handler.h"

#include <entt/entt.hpp>

#include "bedrock/threading.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/level.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_chat_event.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"
#include "endstone/level.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

namespace {
endstone::Level &getLevel(LevelGameplayHandler *handler)
{
    // Find out the level where this event is occurring
    auto &server = entt::locator<EndstoneServer>::value();
    auto levels = server.getLevels();
    auto it = std::find_if(begin(levels), end(levels), [handler](auto *level) {
        return &(static_cast<EndstoneLevel *>(level)
                     ->getBedrockLevel()
                     .getLevelEventCoordinator()
                     .getLevelGameplayHandler()) == handler;
    });
    if (it == levels.end()) {
        throw std::runtime_error("Unable to find level associated with the provided LevelGameplayHandler");
    }
    auto &level = **it;
    return level;
}
}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptLevelGameplayHandler::handleEvent(LevelWeatherChangedEvent &event)
{
    auto &server = entt::locator<EndstoneServer>::value();
    // TODO(improvement): this - 1 is bit hacky. Any better workaround for multiple inheritance?
    auto &level = getLevel(this - 1);

    if (event.from_rain != event.to_rain) {
        endstone::WeatherChangeEvent e(level, event.to_rain);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.to_rain = event.from_rain;
        }
    }

    if (event.from_lightning != event.to_lightning) {
        endstone::ThunderChangeEvent e(level, event.to_lightning);
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
