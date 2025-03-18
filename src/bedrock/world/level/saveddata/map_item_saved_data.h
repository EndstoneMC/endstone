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

#pragma once

#include <memory>
#include <vector>

#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/saveddata/maps/map_decoration.h"
#include "bedrock/world/level/saveddata/maps/map_item_tracked_actor.h"

struct ClientTerrainPixel {
    std::uint32_t pixel;
    bool assigned_to_map;
    bool sent_to_server;
};
static_assert(sizeof(ClientTerrainPixel) == 8);

class MapItemSavedData {
    using DecorationCollection = std::vector<std::pair<MapItemTrackedActor::UniqueId, std::shared_ptr<MapDecoration>>>;

public:
    MapItemSavedData(ActorUniqueID, bool);

private:
    ActorUniqueID map_id_;
    ActorUniqueID parent_map_id_;
    bool is_fully_explored_;
    bool preview_incomplete_;
    BlockPos origin_;
    DimensionType dimension_;
    std::int8_t scale_;
    std::vector<unsigned int> pixels_;
    std::vector<ClientTerrainPixel> client_pixels_;
    std::vector<std::shared_ptr<MapItemTrackedActor>> tracked_entities_;
    bool unlimited_tracking_;
    bool dirty_for_save_;
    bool dirty_pixel_data_;
    bool locked_;
    DecorationCollection decorations_;
    bool has_dirty_client_pixels_;
    std::unique_ptr<SpinLockImpl> client_sampling_lock_;
    bool needs_resampling_;
    bool is_dlc_world_;
};
