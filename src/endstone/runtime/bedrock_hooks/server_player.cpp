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

#include "bedrock/server/server_player.h"

#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/actor/sign_block_actor.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/block/block.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/level/location.h"
#include "endstone/core/player.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/event/player/player_open_sign_event.h"
#include "endstone/event/player/player_portal_event.h"
#include "endstone/runtime/hook.h"

void ServerPlayer::openSign(const BlockPos &position, bool is_front_side)
{
    auto cause = endstone::PlayerOpenSignEvent::Cause::Unknown;
    if (const auto *open_sign_cause = tryGetComponent<endstone::core::InternalOpenSignCauseComponent>()) {
        cause = open_sign_cause->cause;
    }
    getEntity().removeComponent<endstone::core::InternalOpenSignCauseComponent>();

    auto &block_source = getDimensionBlockSource();
    auto *block_entity = block_source.getBlockEntity(position);
    if (block_entity == nullptr ||
        (block_entity->getType() != BlockActorType::Sign && block_entity->getType() != BlockActorType::HangingSign)) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::openSign, this, position, is_front_side);
        return;
    }

    auto &sign_block_actor = static_cast<SignBlockActor &>(*block_entity);
    const ActorUniqueID locked_for_editing_by = sign_block_actor.getLockedForEditingBy();
    const ActorUniqueID current_player_id = getOrCreateUniqueID();
    if (locked_for_editing_by.isValid() && locked_for_editing_by != current_player_id &&
        getLevel().getPlayer(locked_for_editing_by) != nullptr) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::openSign, this, position, is_front_side);
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (server.getEndstonePluginManager().isEventRegistered<endstone::PlayerOpenSignEvent>()) {
        const auto block = endstone::core::EndstoneBlock::at(block_source, position);
        if (const auto sign = block->captureState(true).as<endstone::Sign>(); sign) {
            endstone::PlayerOpenSignEvent event{
                getEndstoneActor<endstone::core::EndstonePlayer>(),
                sign,
                is_front_side ? endstone::Sign::Side::Front : endstone::Sign::Side::Back,
                cause,
            };
            server.getPluginManager().callEvent(event);
            if (event.isCancelled()) {
                return;
            }
        }
    }

    if (locked_for_editing_by == current_player_id) {
        sign_block_actor.setLockedForEditingBy(ActorUniqueID::INVALID_ID);
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::openSign, this, position, is_front_side);
}

void ServerPlayer::changeDimension(DimensionType to_id)
{
    auto to_dimension = getLevel().getOrCreateDimension(to_id);
    if (!to_dimension.isSet()) {
        return;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
    ChangeDimensionRequest request(getDimensionId(), to_id, getPosition(), Vec3::ZERO, true, false);
    static_cast<IPlayerDimensionTransferer &>(
        getLevel().getPlayerDimensionTransferManager()->getPlayerDimensionTransferConnector())
        .setTransitionLocation(*this, request, *to_dimension.unwrap());

    const auto from_location =
        endstone::core::EndstoneLocation::toEndstone(request.from_position, request.from_dimension);
    const auto to_location = endstone::core::EndstoneLocation::toEndstone(request.to_position, request.to_dimension);
    endstone::PlayerPortalEvent e(player, from_location, to_location);
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::changeDimension, this, to_id);
    // request.to_position = Vec3::ZERO;
    // _setDimensionTransitionComponent(getDimensionId(), to_id, 300);
    // getLevel().requestPlayerChangeDimension(*this, std::move(request));
}
