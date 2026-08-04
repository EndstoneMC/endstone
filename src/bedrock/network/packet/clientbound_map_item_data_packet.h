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
public:
    enum class Type : int32_t {
        Invalid = 0,
        TextureUpdate = 2,
        DecorationUpdate = 4,
        Creation = 8,
    };

    ClientboundMapItemDataPacket();
    ClientboundMapItemDataPacket(ActorUniqueID, int8_t, const MapItemSavedData::DecorationCollection &,
                                 buffer_span<unsigned int>, int, int, int, int, DimensionType, bool, const BlockPos &);
    ClientboundMapItemDataPacket(gsl::not_null<MapItemSavedData *>, Level &);
    ActorUniqueID getMapId() const { return map_id_; }
    const std::vector<ActorUniqueID> &getMapIds() const;
    DimensionType getDimensionId() const { return DimensionType(dimension_); }
    int8_t getScale() const;
    void applyToMap(MapItemSavedData &, const bool) const;
    void resampleClientMap(MapItemSavedData &, BlockSource &, const BlockPos &, int) const;
    bool isTextureUpdate() const;
    bool isOfType(Type) const;
    bool isLocked() const;
    BlockPos getMapOrigin() const;
    bool hasEmptyOrBlackPixels() const;

    // protected: ENDSTONE: protected -> public
    // ClientboundMapItemDataPacketPayload, inlined at +48
    ActorUniqueID map_id_;                                    // +48
    Type type_;                                               // +56
    uint8_t dimension_;                                       // +60
    bool locked_;                                             // +61
    BlockPos map_origin_;                                     // +64
    std::vector<ActorUniqueID> map_ids_;                      // +80, only filled on the creation path
    int8_t scale_;                                            // +104
    std::vector<MapItemTrackedActor::UniqueId> unique_ids_;   // +112
    std::vector<std::shared_ptr<MapDecoration>> decorations_;  // +136
    int width_;                                               // +160
    int height_;                                              // +164
    int start_x_;                                             // +168
    int start_y_;                                             // +172
    std::vector<unsigned int> map_pixels_;                    // +176
    SerializationMode serialization_mode{SerializationMode::CerealOnly};  // +200
};
BEDROCK_STATIC_ASSERT_SIZE(ClientboundMapItemDataPacket, 208, 208);

inline ClientboundMapItemDataPacket::Type operator|(ClientboundMapItemDataPacket::Type a,
                                                    ClientboundMapItemDataPacket::Type b)
{
    return static_cast<ClientboundMapItemDataPacket::Type>(static_cast<int32_t>(a) | static_cast<int32_t>(b));
}
