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

#include "endstone/core/event/handlers/level_gameplay_handler.h"

#include <entt/entt.hpp>

#include "bedrock/gameplayhandlers/gameplay_handler_result.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/events/level_events.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_spawn_event.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"

namespace endstone::core {

EndstoneLevelGameplayHandler::EndstoneLevelGameplayHandler(std::unique_ptr<LevelGameplayHandler> handle)
    : handle_(std::move(handle))
{
}

HandlerResult EndstoneLevelGameplayHandler::handleEvent(LevelGameplayEvent<void> const &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const LevelAddedActorEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

GameplayHandlerResult<CoordinatorResult> EndstoneLevelGameplayHandler::handleEvent(
    MutableLevelGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        if (!handleEvent(arg.value())) {
            return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
        }
        return handle_->handleEvent(event);
    };
    return std::visit(visitor, event.variant);
}

bool EndstoneLevelGameplayHandler::handleEvent(const LevelAddedActorEvent &event)
{
    const StackResultStorageEntity stack_result(event.actor);
    if (auto *actor = ::Actor::tryGetFromEntity(stack_result.getStackRef(), false); actor && !actor->isPlayer()) {
        const auto &server = entt::locator<EndstoneServer>::value();
        ActorSpawnEvent e{actor->getEndstoneActor()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            actor->despawn();
        }
    }
    return true;
}

bool EndstoneLevelGameplayHandler::handleEvent(LevelWeatherChangedEvent &event)
{
    const auto &server = entt::locator<EndstoneServer>::value();
    auto &level = *server.getLevel();
    if (event.is_raining != event.will_be_raining) {
        WeatherChangeEvent e(level, event.will_be_raining);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.will_be_raining = event.is_raining;
        }
    }
    if (event.is_lightning != event.will_be_lightning) {
        ThunderChangeEvent e(level, event.will_be_lightning);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.will_be_lightning = event.is_lightning;
        }
    }
    return true;
}

}  // namespace endstone::core
