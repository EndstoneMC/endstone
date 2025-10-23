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

#include "bedrock/core/utility/buffer_span.h"
#include "bedrock/world/level/saveddata/map_item_saved_data.h"

class ClientboundMapItemDataPacket : public Packet {
    enum class Type : int32_t {
        Invalid = 0,
        TextureUpdate = 2,
        DecorationUpdate = 4,
        Creation = 8,
    };

public:
    ClientboundMapItemDataPacket();
    ClientboundMapItemDataPacket(ActorUniqueID, int8_t, const MapItemSavedData::DecorationCollection &,
                                 buffer_span<unsigned int>, int, int, int, int, DimensionType, bool, const BlockPos &);
    ClientboundMapItemDataPacket(gsl::not_null<MapItemSavedData *>, Level &);
    ActorUniqueID getMapId() const
    {
        return map_ids_.front();
    }
    const std::vector<ActorUniqueID> &getMapIds() const;
    DimensionType getDimensionId() const
    {
        return DimensionType(dimension_);
    }
    int8_t getScale() const;
    void applyToMap(MapItemSavedData &, const bool) const;
    void resampleClientMap(MapItemSavedData &, BlockSource &, const BlockPos &, int) const;
    bool isTextureUpdate() const;
    bool isOfType(Type) const;
    bool isLocked() const;
    BlockPos getMapOrigin() const;
    bool hasEmptyOrBlackPixels() const;

    // protected: ENDSTONE: protected -> public
    std::vector<ActorUniqueID> map_ids_;
    int8_t scale_;
    std::vector<std::shared_ptr<MapDecoration>> decorations_;
    std::vector<MapItemTrackedActor::UniqueId> unique_ids_;
    int start_x_;
    int start_y_;
    BlockPos map_origin_;
    uint8_t dimension_;
    int width_;
    int height_;
    Type type_;
    std::vector<unsigned int> map_pixels_;
    bool locked_;
};
static_assert(sizeof(ClientboundMapItemDataPacket) == 200);
