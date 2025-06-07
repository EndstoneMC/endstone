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

#include "endstone/core/map/map_view.h"

#include "endstone/core/map/map_renderer.h"
#include "endstone/core/server.h"

namespace endstone::core {
EndstoneMapView::EndstoneMapView(MapItemSavedData &map) : map_(map)
{
    EndstoneMapView::addRenderer(std::make_unique<EndstoneMapRenderer>(*this, map));
}

std::int64_t EndstoneMapView::getId() const
{
    return map_.getMapId().raw_id;
}

bool EndstoneMapView::isVirtual() const
{
    return !renderers_.empty() && !renderers_.at(0)->isEndstoneMapRenderer();
}

MapView::Scale EndstoneMapView::getScale() const
{
    return static_cast<Scale>(map_.getScale());
}

void EndstoneMapView::setScale(Scale scale)
{
    map_.setScale(static_cast<int>(scale));
}

int EndstoneMapView::getCenterX() const
{
    return map_.getOrigin().x;
}

int EndstoneMapView::getCenterZ() const
{
    return map_.getOrigin().z;
}

void EndstoneMapView::setCenterX(const int x)
{
    if (map_.getOrigin().x != x) {
        map_.origin_.x = x;
        map_.setDirtyForSaveAndPixelData();
    }
}

void EndstoneMapView::setCenterZ(const int z)
{
    if (map_.getOrigin().z != z) {
        map_.origin_.z = z;
        map_.setDirtyForSaveAndPixelData();
    }
}

Dimension *EndstoneMapView::getDimension() const
{

    const auto &server = entt::locator<EndstoneServer>::value();
    const auto &level = *static_cast<EndstoneLevel *>(server.getLevel());
    const auto dimension = level.getHandle().getDimension(map_.getDimensionId());
    if (!dimension.isSet()) {
        return nullptr;
    }
    return &dimension.unwrap()->getEndstoneDimension();
}

void EndstoneMapView::setDimension(const Dimension &dimension)
{
    map_.setDimensionId({static_cast<int>(dimension.getType())});
}

std::vector<MapRenderer *> EndstoneMapView::getRenderers() const
{
    std::vector<MapRenderer *> result;
    for (const auto &renderer : renderers_) {
        result.push_back(renderer.get());
    }
    return result;
}

void EndstoneMapView::addRenderer(std::shared_ptr<MapRenderer> renderer)
{
    renderer->initialize(*this);
    renderers_.emplace_back(std::move(renderer));
    // TODO(map): create canvas entry
}

bool EndstoneMapView::removeRenderer(MapRenderer *renderer)
{
    auto it = std::remove_if(renderers_.begin(), renderers_.end(),
                             [=](const std::shared_ptr<MapRenderer> &ptr) { return ptr.get() == renderer; });
    if (it != renderers_.end()) {
        renderers_.erase(it, renderers_.end());
        // TODO(map): clear canvas
        return true;
    }
    return false;
}

bool EndstoneMapView::isUnlimitedTracking() const
{
    return map_.unlimited_tracking_;
}

void EndstoneMapView::setUnlimitedTracking(const bool unlimited)
{
    if (map_.unlimited_tracking_ != unlimited) {
        map_.unlimited_tracking_ = unlimited;
        map_.setDirtyForSaveAndPixelData();
    }
}

bool EndstoneMapView::isLocked() const
{
    return map_.isLocked();
}

void EndstoneMapView::setLocked(const bool locked)
{
    if (map_.isLocked() != locked) {
        map_.locked_ = locked;
        map_.setDirtyForSaveAndPixelData();
    }
}

bool EndstoneMapView::isContextual() const
{
    return std::ranges::any_of(renderers_, [](const auto &renderer) { return renderer->isContextual(); });
}
}  // namespace endstone::core
