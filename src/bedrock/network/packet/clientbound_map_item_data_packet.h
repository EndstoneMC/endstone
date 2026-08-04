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

struct ClientboundMapItemDataPacketPayload {
    enum class Type : int32_t {
        Invalid = 0,
        TextureUpdate = 2,
        DecorationUpdate = 4,
        Creation = 8,
    };

    ActorUniqueID map_id;                                     // +0
    Type type;                                                // +8
    uint8_t dimension;                                        // +12
    bool locked;                                              // +13
    BlockPos map_origin;                                      // +16
    std::vector<ActorUniqueID> map_ids;                       // +32, only filled on the creation path
    int8_t scale;                                             // +56
    std::vector<MapItemTrackedActor::UniqueId> unique_ids;    // +64
    std::vector<std::shared_ptr<MapDecoration>> decorations;  // +88
    int width;                                                // +112
    int height;                                               // +116
    int start_x;                                              // +120
    int start_y;                                              // +124
    std::vector<unsigned int> map_pixels;                     // +128
};
BEDROCK_STATIC_ASSERT_SIZE(ClientboundMapItemDataPacketPayload, 152, 152);

class ClientboundMapItemDataPacket : public Packet {
public:
    using Type = ClientboundMapItemDataPacketPayload::Type;

    ClientboundMapItemDataPacket();
    ClientboundMapItemDataPacket(ActorUniqueID, int8_t, const MapItemSavedData::DecorationCollection &,
                                 buffer_span<unsigned int>, int, int, int, int, DimensionType, bool, const BlockPos &);
    ClientboundMapItemDataPacket(gsl::not_null<MapItemSavedData *>, Level &);
    ActorUniqueID getMapId() const { return payload.map_id; }
    const std::vector<ActorUniqueID> &getMapIds() const;
    DimensionType getDimensionId() const { return DimensionType(payload.dimension); }
    int8_t getScale() const;
    void applyToMap(MapItemSavedData &, const bool) const;
    void resampleClientMap(MapItemSavedData &, BlockSource &, const BlockPos &, int) const;
    bool isTextureUpdate() const;
    bool isOfType(Type) const;
    bool isLocked() const;
    BlockPos getMapOrigin() const;
    bool hasEmptyOrBlackPixels() const;

    // protected: ENDSTONE: protected -> public
    ClientboundMapItemDataPacketPayload payload;                          // +48
    SerializationMode serialization_mode{SerializationMode::CerealOnly};  // +200
};
BEDROCK_STATIC_ASSERT_SIZE(ClientboundMapItemDataPacket, 208, 208);

inline ClientboundMapItemDataPacket::Type operator|(ClientboundMapItemDataPacket::Type a,
                                                    ClientboundMapItemDataPacket::Type b)
{
    return static_cast<ClientboundMapItemDataPacket::Type>(static_cast<int32_t>(a) | static_cast<int32_t>(b));
}
