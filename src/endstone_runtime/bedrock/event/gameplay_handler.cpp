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

#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"
#include "endstone/detail/virtual_table.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"
#include "endstone/level/level.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;
using endstone::detail::VirtualTable;

namespace endstone::detail {

class EndstoneLevelGameplayHandler : public ::LevelGameplayHandler {

    static Level &getLevel(LevelGameplayHandler &handler_)
    {
        // Find out the level where this event is occurring
        auto &server = entt::locator<EndstoneServer>::value();
        auto levels = server.getLevels();
        auto it = std::find_if(begin(levels), end(levels), [&handler_](auto *level) {
            return &(static_cast<EndstoneLevel *>(level)
                         ->getBedrockLevel()
                         .getLevelEventCoordinator()
                         .getLevelGameplayHandler()) == &handler_;
        });
        if (it == levels.end()) {
            server.getLogger().critical("Unable to find level associated with the provided LevelGameplayHandler");
            std::terminate();
        }
        auto &level = **it;
        return level;
    }

public:
    GameplayHandlerResult<CoordinatorResult> handleEvent(LevelWeatherChangedEvent &event)
    {
        auto &vtable = entt::locator<VirtualTable<ScriptLevelGameplayHandler>>::value();

#ifdef _WIN32
        GameplayHandlerResult<CoordinatorResult> result;
        vtable.call<3, GameplayHandlerResult<CoordinatorResult>>(this, &result, event);
#else
        auto result = vtable.call<3, GameplayHandlerResult<CoordinatorResult>>(this, event);
#endif

        if (result.value == CoordinatorResult::Deny || result.result == EventResult::Deny) {
            return result;
        }

        auto &server = entt::locator<EndstoneServer>::value();
        auto &level = getLevel(*this);

        if (event.from_rain != event.to_rain) {
            server.getLogger().debug("Weather state was changed in level: {}", level.getName());
            endstone::WeatherChangeEvent e(level, event.to_rain);
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                event.to_rain = event.from_rain;
            }
        }

        if (event.from_lightning != event.to_lightning) {
            server.getLogger().debug("Thunder state was changed in level: {}", level.getName());
            endstone::ThunderChangeEvent e(level, event.to_lightning);
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                event.to_lightning = event.from_lightning;
            }
        }

        return result;
    }

    HandlerResult handleEvent(LevelStartLeaveGameEvent const &event)
    {
        auto &server = entt::locator<EndstoneServer>::value();
        auto &level = getLevel(*this);
        server.getLogger().debug("Unloading level: {}. Game is being shut down.", level.getName());
        auto &vtable = entt::locator<VirtualTable<ScriptLevelGameplayHandler>>::value();
        return vtable.call<6, HandlerResult>(this, event);
    }

    HandlerResult handleEvent(LevelAddedActorEvent const &event)
    {
        auto &server = entt::locator<EndstoneServer>::value();
        auto &level = getLevel(*this);
        server.getLogger().debug("An actor has been added to level: {}.", level.getName());
        auto &vtable = entt::locator<VirtualTable<ScriptLevelGameplayHandler>>::value();
        return vtable.call<10, HandlerResult>(this, event);
    }
};

template <>
void VirtualTableHook<ScriptLevelGameplayHandler>::init()
{
    hook<3, LevelWeatherChangedEvent &>(&EndstoneLevelGameplayHandler::handleEvent);
    // 4: handleEvent(ScriptingInitializeEvent const&)
    // 5: handleEvent(LevelGameRuleChangeEvent const&)
    hook<6, LevelStartLeaveGameEvent const &>(&EndstoneLevelGameplayHandler::handleEvent);
    // 7: handleEvent(LevelDayCycleEvent const&)
    // 8: handleEvent(LevelSoundBroadcastEvent const&)
    // 9: handleEvent(LevelBroadcastEvent const&)
    hook<10, LevelAddedActorEvent const &>(&EndstoneLevelGameplayHandler::handleEvent);
}

}  // namespace endstone::detail
