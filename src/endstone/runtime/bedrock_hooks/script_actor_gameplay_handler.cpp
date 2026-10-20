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

#include <utility>
#include <variant>

#include "bedrock/entity/components/container_component.h"
#include "bedrock/server/server_player.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/actor/item.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/block/block.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/actor_inventory.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_change_block_event.h"
#include "endstone/event/actor/actor_damage_event.h"
#include "endstone/event/actor/actor_death_event.h"
#include "endstone/event/actor/actor_dismount_event.h"
#include "endstone/event/actor/actor_effect_event.h"
#include "endstone/event/actor/actor_pickup_item_event.h"
#include "endstone/event/actor/actor_remove_event.h"
#include "endstone/event/inventory/inventory_close_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(const ActorDiedEvent &event)
{
    if (const auto *mob = event.entity.tryUnwrap<::Mob>(); mob) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::ActorDeathEvent e{mob->getEndstoneActor<endstone::core::EndstoneMob>(),
                                    std::make_shared<endstone::core::EndstoneDamageSource>(*event.source)};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

void closeInventoryViewers(::Actor &actor)
{
    if (!std::as_const(actor.getEntity()).tryGetComponent<ContainerComponent>()) {
        return;
    }
    const auto inventory = std::make_shared<endstone::core::EndstoneActorInventory>(actor.getWeakEntity());
    const auto id = actor.getOrCreateUniqueID();
    actor.getLevel().forEachPlayer([&](::Player &player) {
        const auto *open = player.getEntity().tryGetComponent<endstone::core::InternalOpenContainerComponent>();
        const auto *owner = open == nullptr ? nullptr : std::get_if<ActorUniqueID>(&open->owner);
        if (owner == nullptr || *owner != id) {
            return true;
        }
        player.getEntity().removeComponent<endstone::core::InternalOpenContainerComponent>();
        const auto e = std::make_shared<endstone::InventoryCloseEvent>(
            inventory, player.getEndstoneActor<endstone::core::EndstonePlayer>());
        endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(*e);
        static_cast<ServerPlayer &>(player).initiateContainerClose();
        return true;
    });
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
        closeInventoryViewers(*actor);
    }
    return true;
}

bool handleEvent(::ActorBeforeHurtEvent &event)
{
    const auto &source = event.source;
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto mob = event.entity.getEndstoneActor<endstone::core::EndstoneMob>();
    endstone::ActorDamageEvent e{mob, std::make_shared<endstone::core::EndstoneDamageSource>(source), event.damage};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    if (e.getDamage() != event.damage) {
        event.damage = e.getDamage();
        event.was_modified = true;
    }
    return true;
}

bool handleEvent(::ActorAddEffectEvent &event)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::ActorEffectEvent>()) {
        return true;
    }

    auto *mob = WeakEntityRef(event.entity).tryUnwrap<::Mob>();
    if (!mob) {
        return true;
    }

    const endstone::EffectId type{endstone::EffectId::Minecraft, event.mob_effect.getResourceName()};
    const endstone::Effect effect{
        type,
        event.mob_effect.getDuration().getValue(),
        event.mob_effect.getAmplifier(),
        event.mob_effect.isAmbient(),
        event.mob_effect.isEffectVisible(),
        event.mob_effect.displaysOnScreenTextureAnimation(),
    };

    endstone::ActorEffectEvent e{mob->getEndstoneActor<endstone::core::EndstoneMob>(),
                                 endstone::ActorEffectEvent::Action::Added, type, std::nullopt, effect};
    server.getPluginManager().callEvent(e);
    return !e.isCancelled();
}

bool handleEvent(::ActorBeforeAcquireItemEvent &event)
{
    if (event.acquire_method != ItemAcquisitionMethod::PickedUp || event.actor.isPlayer() ||
        !event.item.hasCategory(ActorCategory::Item)) {
        // Players are covered by PlayerPickupItemEvent
        return true;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::ActorPickupItemEvent>()) {
        return true;
    }

    endstone::ActorPickupItemEvent e{event.actor.getEndstoneActor(),
                                     event.item.getEndstoneActor<endstone::core::EndstoneItem>(),
                                     event.amount_acquired};
    server.getPluginManager().callEvent(e);
    return !e.isCancelled();
}

bool handleEvent(const ActorStopRidingEvent &event)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::ActorDismountEvent>()) {
        return true;
    }

    auto *passenger = WeakEntityRef(event.passenger).tryUnwrap<::Actor>();
    auto *vehicle = WeakEntityRef(event.vehicle).tryUnwrap<::Actor>();
    if (!passenger || !vehicle) {
        return true;
    }

    endstone::ActorDismountEvent e{passenger->getEndstoneActor(), vehicle->getEndstoneActor()};
    server.getPluginManager().callEvent(e);
    return !e.isCancelled();
}

bool handleEvent(const ActorGriefingBlockEvent &event)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::ActorChangeBlockEvent>()) {
        return true;
    }

    auto *actor = WeakEntityRef(event.actor_context).tryUnwrap<::Actor>();
    if (!actor || !event.block_source_handle || !*event.block_source_handle) {
        return true;
    }

    endstone::ActorChangeBlockEvent e{
        actor->getEndstoneActor(), endstone::core::EndstoneBlock::at(**event.block_source_handle, BlockPos(event.pos))};
    server.getPluginManager().callEvent(e);
    return !e.isCancelled();
}
}  // namespace

HandlerResult ScriptActorGameplayHandler::handleEvent1(const ActorGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const ActorDiedEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ActorRemovedEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptActorGameplayHandler::handleEvent1, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptActorGameplayHandler::handleEvent3(
    const ActorGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const ActorStopRidingEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ActorGriefingBlockEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptActorGameplayHandler::handleEvent3, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptActorGameplayHandler::handleEvent4(
    MutableActorGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<::ActorBeforeHurtEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<::ActorAddEffectEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<::ActorBeforeAcquireItemEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptActorGameplayHandler::handleEvent4, this, event);
    };
    return event.visit(visitor);
}
