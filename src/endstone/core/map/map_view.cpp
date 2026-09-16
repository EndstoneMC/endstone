
#include <ranges>
#include <unordered_map>
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
#include "endstone/core/map/map_renderer.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"

namespace endstone::core {
EndstoneMapView::EndstoneMapView(ActorUniqueID map_id) : map_id_(map_id)
{
    EndstoneMapView::addRenderer(std::make_unique<EndstoneMapRenderer>());
}

std::int64_t EndstoneMapView::getId() const
{
    return map_id_.raw_id;
}

bool EndstoneMapView::isVirtual() const
{
    return !renderers_.empty() && !renderers_.at(0)->isEndstoneMapRenderer();
}

MapView::Scale EndstoneMapView::getScale() const
{
    return static_cast<Scale>(getHandle().getScale());
}

void EndstoneMapView::setScale(Scale scale)
{
    getHandle().setScale(static_cast<int>(scale));
}

int EndstoneMapView::getCenterX() const
{
    return getHandle().getOrigin().x;
}

int EndstoneMapView::getCenterZ() const
{
    return getHandle().getOrigin().z;
}

void EndstoneMapView::setCenterX(const int x)
{
    auto &map = getHandle();
    if (map.getOrigin().x != x) {
        map.origin_.x = x;
        map.setDirtyForSaveAndPixelData();
    }
}

void EndstoneMapView::setCenterZ(const int z)
{
    auto &map = getHandle();
    if (map.getOrigin().z != z) {
        map.origin_.z = z;
        map.setDirtyForSaveAndPixelData();
    }
}

Dimension *EndstoneMapView::getDimension() const
{
    const auto *level = EndstoneServer::getInstance().getEndstoneLevel();
    const auto dimension = level->getHandle().getDimension(getHandle().getDimensionId());
    if (!dimension.isSet()) {
        return nullptr;
    }
    return level->getDimension(dimension.unwrap()->getDimensionId().value);
}

void EndstoneMapView::setDimension(const Dimension &dimension)
{
    getHandle().setDimensionId({static_cast<int>(dimension.getType())});
}

std::vector<std::shared_ptr<MapRenderer>> EndstoneMapView::getRenderers() const
{
    return renderers_;
}

void EndstoneMapView::addRenderer(std::shared_ptr<MapRenderer> renderer)
{
    auto it = std::ranges::find_if(renderers_, [&](const auto &r) { return renderer == r; });
    if (it == renderers_.end()) {
        renderers_.emplace_back(renderer);
        canvases_[renderer] = std::unordered_map<std::uint64_t, std::unique_ptr<EndstoneMapCanvas>>();
        renderer->initialize(*this);
    }
}

bool EndstoneMapView::removeRenderer(const std::shared_ptr<MapRenderer> &renderer)
{
    auto it = std::ranges::find_if(renderers_, [&](const auto &r) { return renderer == r; });
    if (it != renderers_.end()) {
        renderers_.erase(it);
        canvases_.erase(renderer);
        return true;
    }
    return false;
}

bool EndstoneMapView::isUnlimitedTracking() const
{
    return getHandle().unlimited_tracking_;
}

void EndstoneMapView::setUnlimitedTracking(const bool unlimited)
{
    auto &map = getHandle();
    if (map.unlimited_tracking_ != unlimited) {
        map.unlimited_tracking_ = unlimited;
        // map.setDirtyForSaveAndPixelData();
    }
}

bool EndstoneMapView::isLocked() const
{
    return getHandle().isLocked();
}

void EndstoneMapView::setLocked(const bool locked)
{
    auto &map = getHandle();
    if (map.isLocked() != locked) {
        map.locked_ = locked;
        // map.setDirtyForSaveAndPixelData();
    }
}

const RenderData &EndstoneMapView::render(EndstonePlayer &player)
{
    bool context = isContextual();
    auto unique_id = context ? player.getId() : ActorUniqueID::INVALID_ID.raw_id;
    RenderData &render = render_cache_.emplace(unique_id, RenderData()).first->second;
    if (context) {
        render_cache_.erase(ActorUniqueID::INVALID_ID.raw_id);
    }

    std::ranges::fill(render.buffer, 0);
    render.cursors.clear();

    for (const auto &renderer : renderers_) {
        auto &canvas = *canvases_.at(renderer)
                            .emplace(renderer->isContextual() ? player.getId() : ActorUniqueID::INVALID_ID.raw_id,
                                     std::make_unique<EndstoneMapCanvas>(*this))
                            .first->second;

        canvas.setBase(render.buffer);
        try {
            renderer->render(*this, canvas, player);
        }
        catch (std::exception &e) {
            player.getServer().getLogger().critical("Could not render map: {}", e.what());
        }
        render.buffer = canvas.getBuffer();

        for (const auto &cursor : canvas.getCursors()) {
            render.cursors.emplace_back(cursor);
        }
    }
    return render;
}

MapItemSavedData &EndstoneMapView::getHandle() const
{
    auto *map = EndstoneServer::getInstance().getEndstoneLevel()->getHandle().getMapSavedData(map_id_);
    if (!map) {
        throw std::runtime_error("Trying to access a map that is no longer valid.");
    }
    return *map;
}

bool EndstoneMapView::isContextual() const
{
    return std::ranges::any_of(renderers_, [](const auto &renderer) { return renderer->isContextual(); });
}
}  // namespace endstone::core
