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

#include "endstone/core/map/map_canvas.h"

#include "bedrock/world/item/map_constants.h"
#include "endstone/core/map/map_view.h"

namespace endstone::core {
EndstoneMapCanvas::EndstoneMapCanvas(EndstoneMapView &map_view)
    : buffer_(MapConstants::MAP_SIZE * MapConstants::MAP_SIZE, 0), map_view_(map_view)
{
}

MapView &EndstoneMapCanvas::getMapView() const
{
    return map_view_;
}

std::vector<MapCursor> EndstoneMapCanvas::getCursors() const
{
    return cursors_;
}

void EndstoneMapCanvas::setCursors(const std::vector<MapCursor> &cursors)
{
    cursors_ = cursors;
}

void EndstoneMapCanvas::setPixelColor(int x, int y, Color color)
{
    setPixel(x, y, color.asABGR());
}

Color EndstoneMapCanvas::getPixelColor(int x, int y) const
{
    return Color::fromABGR(static_cast<std::int32_t>(getPixel(x, y)));
}

Color EndstoneMapCanvas::getBasePixelColor(int x, int y) const
{
    return Color::fromABGR(static_cast<std::int32_t>(getBasePixel(x, y)));
}

void EndstoneMapCanvas::setPixel(int x, int y, std::uint32_t color)
{
    if (x < 0 || y < 0 || x >= MapConstants::MAP_SIZE || y >= MapConstants::MAP_SIZE) {
        return;
    }
    if (buffer_[y * MapConstants::MAP_SIZE + x] != color) {
        buffer_[y * MapConstants::MAP_SIZE + x] = color;
        map_view_.map_.setPixelDirty(x, y);
    }
}

std::uint32_t EndstoneMapCanvas::getPixel(int x, int y) const
{
    if (x < 0 || y < 0 || x >= MapConstants::MAP_SIZE || y >= MapConstants::MAP_SIZE) {
        return 0;
    }
    return buffer_[y * MapConstants::MAP_SIZE + x];
}

std::uint32_t EndstoneMapCanvas::getBasePixel(int x, int y) const
{
    if (base_ == nullptr) {
        return 0;
    }
    if (x < 0 || y < 0 || x >= MapConstants::MAP_SIZE || y >= MapConstants::MAP_SIZE) {
        return 0;
    }
    return (*base_)[y * MapConstants::MAP_SIZE + x];
}

void EndstoneMapCanvas::drawImage(int x, int y, const Image &image)
{
    for (auto xx = 0; xx < image.getWidth(); ++xx) {
        for (auto yy = 0; yy < image.getHeight(); ++yy) {
            setPixelColor(x + xx, y + yy, image.getColor(xx, yy));
        }
    }
}

void EndstoneMapCanvas::setBase(const std::vector<std::uint32_t> &base)
{
    base_ = &base;
}

const std::vector<std::uint32_t> &EndstoneMapCanvas::getBuffer() const
{
    return buffer_;
}
}  // namespace endstone::core
