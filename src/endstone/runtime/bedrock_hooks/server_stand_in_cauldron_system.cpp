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

#include "bedrock/entity/systems/server_stand_in_cauldron_system.h"

#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/actor/actor_environment.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/cauldron_level_change_event.h"
#include "endstone/runtime/hook.h"

void ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron(ActorOwnerComponent &owner)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, owner);
        return;
    }

    auto &actor = owner.getActor();
    auto level_entity = actor.getLevel().getLevelEntity().unwrap();
    if (!level_entity) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, owner);
        return;
    }

    auto &interaction = level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>();
    interaction.source = &actor;
    interaction.reason = endstone::CauldronLevelChangeEvent::ChangeReason::Extinguish;
    interaction.any_pos = true;
    auto &entity = actor.getEntity();
    const auto was_in_water = ActorEnvironment::getIsInWater(entity);
    const auto fall_distance = actor.getFallDistance();
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, owner);
    if (level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>().cancelled) {
        if (!was_in_water) {
            entity.removeComponent<WasInWaterFlagComponent>();
        }
        actor.setFallDistance(fall_distance);
    }
    level_entity->removeComponent<endstone::core::InternalCauldronInteractionComponent>();
}
