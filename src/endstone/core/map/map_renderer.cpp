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
#include "endstone/core/map/map_canvas.h"
#include "endstone/player.h"

namespace endstone::core {
EndstoneMapRenderer::EndstoneMapRenderer() : MapRenderer(false) {}

void EndstoneMapRenderer::render(MapView &map, MapCanvas &canvas, Player &player)
{
    auto &handle = static_cast<EndstoneMapView &>(map).getHandle();

    // Map
    auto pixels = handle.getPixels();
    for (auto x = 0; x < MapConstants::MAP_SIZE; ++x) {
        for (auto y = 0; y < MapConstants::MAP_SIZE; ++y) {
            canvas.setPixel(x, y, pixels[y * MapConstants::MAP_SIZE + x]);
        }
    }

    // Cursors
    std::vector<MapCursor> cursors;
    std::vector<MapItemTrackedActor::UniqueId> cursor_ids;
    for (const auto &[unique_id, decoration] : handle.getDecorations()) {
        cursors.emplace_back(decoration->getX(), decoration->getY(), decoration->getRot(),
                             static_cast<MapCursor::Type>(decoration->getImg()), true, decoration->getLabel());
        cursor_ids.push_back(unique_id);
    }
    canvas.setCursors(cursors);
    static_cast<EndstoneMapCanvas &>(canvas).setCursorIds(std::move(cursor_ids));
}
}  // namespace endstone::core
