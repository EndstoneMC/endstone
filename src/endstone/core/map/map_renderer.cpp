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

#include "endstone/core/map/map_renderer.h"

#include "bedrock/world/item/map_constants.h"

namespace endstone::core {
EndstoneMapRenderer::EndstoneMapRenderer(EndstoneMapView &map_view, MapItemSavedData &map)
    : MapRenderer(false), map_(map)
{
}

void EndstoneMapRenderer::render(MapView &map, MapCanvas &canvas, Player &player)
{
    // Map
    auto pixels = map_.getPixels();
    for (auto x = 0; x < MapConstants::MAP_SIZE; ++x) {
        for (auto y = 0; y < MapConstants::MAP_SIZE; ++y) {
            canvas.setPixel(x, y, pixels[y * MapConstants::MAP_SIZE + x]);
        }
    }

    // TODO(map): Cursors
}
}  // namespace endstone::core
