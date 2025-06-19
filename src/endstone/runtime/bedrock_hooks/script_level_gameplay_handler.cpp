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

#include "bedrock/scripting/event_handlers/script_level_gameplay_handler.h"

#include <entt/entt.hpp>

#include "bedrock/gameplayhandlers/gameplay_handler_result.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/events/level_events.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_spawn_event.h"
#include "endstone/event/weather/thunder_change_event.h"
#include "endstone/event/weather/weather_change_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(const LevelAddedActorEvent &event)
{
    if (auto *actor = WeakEntityRef(event.actor).tryUnwrap<::Actor>(); actor && !actor->isPlayer()) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::ActorSpawnEvent e{actor->getEndstoneActor()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            actor->addOrRemoveComponent<endstone::core::JustSpawnedFlagComponent>(true);
            actor->despawn();
        }
    }
    return true;
}

bool handleEvent(LevelWeatherChangedEvent &event)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &level = *server.getLevel();
    if (event.is_raining != event.will_be_raining) {
        endstone::WeatherChangeEvent e(level, event.will_be_raining);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.will_be_raining = event.is_raining;
        }
    }
    if (event.is_lightning != event.will_be_lightning) {
        endstone::ThunderChangeEvent e(level, event.will_be_lightning);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            event.will_be_lightning = event.is_lightning;
        }
    }
    return true;
}
}  // namespace

HandlerResult ScriptLevelGameplayHandler::handleEvent1(LevelGameplayEvent<void> const &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const LevelAddedActorEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptLevelGameplayHandler::handleEvent1, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptLevelGameplayHandler::handleEvent2(
    MutableLevelGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        if (!handleEvent(arg.value())) {
            return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptLevelGameplayHandler::handleEvent2, this, event);
    };
    return event.visit(visitor);
}
