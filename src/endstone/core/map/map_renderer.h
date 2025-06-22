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

#include "bedrock/world/level/saveddata/map_item_saved_data.h"
#include "endstone/core/map/map_view.h"
#include "endstone/map/map_renderer.h"

namespace endstone::core {
class EndstoneMapRenderer : public MapRenderer {
public:
    EndstoneMapRenderer(EndstoneMapView &map_view, MapItemSavedData &map);
    void render(MapView &map, MapCanvas &canvas, Player &player) override;

private:
    MapItemSavedData &map_;
};
}  // namespace endstone::core
