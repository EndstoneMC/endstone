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

#include "endstone/map/map_canvas.h"

namespace endstone::core {
class EndstoneMapView;
class EndstoneMapCanvas : public MapCanvas {
public:
    EndstoneMapCanvas(EndstoneMapView &map_view);
    [[nodiscard]] MapView &getMapView() const override;
    [[nodiscard]] std::vector<MapCursor> getCursors() const override;
    void setCursors(const std::vector<MapCursor> &cursors) override;
    void setPixelColor(int x, int y, Color color) override;
    [[nodiscard]] Result<Color> getPixelColor(int x, int y) const override;
    [[nodiscard]] Result<Color> getBasePixelColor(int x, int y) const override;
    void setPixel(int x, int y, std::uint32_t color) override;
    [[nodiscard]] std::uint32_t getPixel(int x, int y) const override;
    [[nodiscard]] std::uint32_t getBasePixel(int x, int y) const override;
    void drawImage(int x, int y, const Image &image) override;

    void setBase(const std::vector<std::uint32_t> &base);
    const std::vector<std::uint32_t> &getBuffer() const;

private:
    std::vector<std::uint32_t> buffer_;
    const std::vector<std::uint32_t> *base_;
    EndstoneMapView &map_view_;
    std::vector<MapCursor> cursors_;
};
}  // namespace endstone::core
