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

#include "bedrock/scripting/event_handlers/script_actor_gameplay_handler.h"

#include "bedrock/world/actor/actor.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_damage_event.h"
#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(const ActorKilledEvent &event)
{
    if (const auto *mob = WeakEntityRef(event.actor_context).tryUnwrap<::Mob>(); mob && !mob->isPlayer()) {
        const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
        endstone::ActorDeathEvent e{mob->getEndstoneActor<endstone::core::EndstoneMob>(),
                                    std::make_unique<endstone::core::EndstoneDamageSource>(*event.source)};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

bool handleEvent(const ActorRemovedEvent &event)
{
    if (auto *actor = WeakEntityRef(event.entity).tryUnwrap<::Actor>(); actor) {
        if (actor->isPlayer()) {
            // Don't call for player
            return true;
        }

        if (actor->hasComponent<endstone::core::InternalRemoveFlagComponent>()) {
            // Don't call if the entity is removed before it is even spawned (when the spawn event is cancelled)
            actor->addOrRemoveComponent<endstone::core::InternalRemoveFlagComponent>(false);
            return true;
        }

        endstone::ActorRemoveEvent e{actor->getEndstoneActor()};
        endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(e);
    }
    return true;
}

bool handleEvent(::ActorBeforeHurtEvent &event)
{
    const auto &source = event.source;
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto &mob = event.entity.getEndstoneActor<endstone::core::EndstoneMob>();
    endstone::ActorDamageEvent e{mob, std::make_unique<endstone::core::EndstoneDamageSource>(source), event.damage};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    event.damage = e.getDamage();
    return true;
}
}  // namespace

HandlerResult ScriptActorGameplayHandler::handleEvent1(const ActorGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const ActorKilledEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ActorRemovedEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptActorGameplayHandler::handleEvent1, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptActorGameplayHandler::handleEvent4(
    MutableActorGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<::ActorBeforeHurtEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptActorGameplayHandler::handleEvent4, this, event);
    };
    return event.visit(visitor);
}
