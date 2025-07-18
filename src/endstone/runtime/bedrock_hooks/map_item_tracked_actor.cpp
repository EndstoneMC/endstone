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

#include "bedrock/world/level/saveddata/maps/map_item_tracked_actor.h"

#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/world/level/saveddata/map_item_saved_data.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

std::unique_ptr<Packet> MapItemTrackedActor::nextUpdatePacket(const MapItemSavedData &saved_data)
{
    auto packet = ENDSTONE_HOOK_CALL_ORIGINAL(&MapItemTrackedActor::nextUpdatePacket, this, saved_data);
    if (!packet) {
        return packet;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto &level = static_cast<endstone::core::EndstoneLevel *>(server.getLevel())->getHandle();
    const auto dimension = level.getDimension(saved_data.getDimensionId());
    const auto *player = getEntity(dimension.unwrap()->getBlockSourceFromMainChunkSource());
    if (!player || !player->isPlayer()) {
        return packet;
    }

    auto &render = saved_data.getMapView().render(player->getEndstoneActor<endstone::core::EndstonePlayer>());
    auto &pk = static_cast<ClientboundMapItemDataPacket &>(*packet);
    pk.map_pixels_ = render.buffer;
    pk.decorations_.clear();
    for (const auto &cursor : render.cursors) {
        if (cursor.isVisible()) {
            pk.decorations_.emplace_back(
                std::make_shared<MapDecoration>(static_cast<MapDecoration::Type>(cursor.getType()), cursor.getX(),
                                                cursor.getY(), cursor.getDirection(), cursor.getCaption(),
                                                mce::Color::WHITE  // TODO(map): support different colors
                                                ));
        }
    }
    return packet;
}
