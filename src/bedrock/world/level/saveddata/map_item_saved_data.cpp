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

#include "bedrock/world/level/saveddata/map_item_saved_data.h"

void MapItemSavedData::setScale(int scale)
{
    if (scale_ != scale) {
        setDirtyForSaveAndPixelData();
        scale_ = scale;
    }
}

void MapItemSavedData::setDimensionId(DimensionType dimension)
{
    dimension_ = dimension;
}

int MapItemSavedData::getScale() const
{
    return scale_;
}

const BlockPos &MapItemSavedData::getOrigin() const
{
    return origin_;
}

DimensionType MapItemSavedData::getDimensionId() const
{
    return dimension_;
}

const MapItemSavedData::DecorationCollection &MapItemSavedData::getDecorations() const
{
    return decorations_;
}

bool MapItemSavedData::isLocked() const
{
    return locked_;
}

void MapItemSavedData::setDirtyForSaveAndPixelData()
{
    dirty_for_save_ = true;
    dirty_pixel_data_ = true;
}

void MapItemSavedData::setPixelDirty(uint32_t x, uint32_t y)
{
    for (const auto &tracked_entity : tracked_entities_) {
        tracked_entity->setPixelDirty(x, y);
    }
}

ActorUniqueID MapItemSavedData::getMapId() const
{
    return map_id_;
}

buffer_span<unsigned int> MapItemSavedData::getPixels() const
{
    return pixels_;
}
