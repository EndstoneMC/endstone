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

#include "endstone/core/actor/actor.h"
#include "endstone/core/level/location.h"
#include "endstone/core/player.h"
#include "endstone/event/player/player_portal_event.h"
#include "endstone/runtime/hook.h"

void ServerPlayer::changeDimension(DimensionType to_id)
{
    auto to_dimension = getLevel().getOrCreateDimension(to_id);
    if (!to_dimension.isSet()) {
        return;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
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
