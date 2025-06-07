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
#include "endstone/map/map_view.h"

namespace endstone::core {
class EndstoneMapView : public MapView {
public:
    explicit EndstoneMapView(MapItemSavedData &map);
    [[nodiscard]] std::int64_t getId() const override;
    [[nodiscard]] bool isVirtual() const override;
    [[nodiscard]] Scale getScale() const override;
    void setScale(Scale scale) override;
    [[nodiscard]] int getCenterX() const override;
    [[nodiscard]] int getCenterZ() const override;
    void setCenterX(int x) override;
    void setCenterZ(int z) override;
    [[nodiscard]] Dimension *getDimension() const override;
    void setDimension(const Dimension &dimension) override;
    [[nodiscard]] std::vector<MapRenderer *> getRenderers() const override;
    void addRenderer(std::shared_ptr<MapRenderer> renderer) override;
    bool removeRenderer(MapRenderer *renderer) override;
    [[nodiscard]] bool isUnlimitedTracking() const override;
    void setUnlimitedTracking(bool unlimited) override;
    [[nodiscard]] bool isLocked() const override;
    void setLocked(bool locked) override;

private:
    bool isContextual() const;

    std::vector<std::shared_ptr<MapRenderer>> renderers_;
    MapItemSavedData &map_;
};
}  // namespace endstone::core
