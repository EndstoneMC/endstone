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

#include <cstdint>

#include <gsl/gsl>

#include "bedrock/bedrock.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/level/block/block_legacy.h"
#include "bedrock/world/level/block/block_serialization_id.h"
#include "bedrock/world/level/block/components/block_component_direct_data.h"
#include "bedrock/world/level/block/components/block_component_storage.h"
#include "bedrock/world/level/block/states/block_state.h"
#include "bedrock/world/level/material/material.h"

enum class BlockOcclusionType : int {
    Unknown = 0,
    HalfSlab = 1,
    Leaf = 2,
    ThinConnects2D = 3,
    Connects2D = 4,
    isLiquid = 5,
    Portal = 6,
    Ice = 7,
    Cactus = 8,
    PowderSnow = 9,
    SculkShrieker = 10,
    Default = 11,
};

struct CachedComponentData {
    Brightness emissive_brightness;     // +0
    bool is_solid;                      // +1
    BlockOcclusionType occlusion_type;  // +4
};

class Block {
public:
    virtual ~Block() = default;

    [[nodiscard]] HashType64 getHashedSerializedId() const;
    [[nodiscard]] std::uint32_t getSerializationIdHashForNetwork() const;
    [[nodiscard]] bool operator!=(const HashType64 &other) const;
    [[nodiscard]] bool operator==(const HashType64 &other) const;
    [[nodiscard]] bool hasProperty(BlockProperty) const;
    [[nodiscard]] Brightness getLightEmission() const;
    [[nodiscard]] float getTranslucency() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] Brightness getLight() const;
    [[nodiscard]] FlameOdds getFlameOdds() const;
    [[nodiscard]] BurnOdds getBurnOdds() const;
    [[nodiscard]] float getExplosionResistance() const;
    [[nodiscard]] bool hasState(const HashedString &name) const;
    bool getCollisionShape(AABB &out_aabb, IConstBlockSource const &region, BlockPos const &pos,
                           optional_ref<GetCollisionShapeInterface const> entity) const;
    bool addCollisionShapes(IConstBlockSource const &region, BlockPos const &pos, AABB const *intersect_test_box,
                            std::vector<AABB> &in_out_boxes,
                            optional_ref<GetCollisionShapeInterface const> entity) const;
    bool getCollisionShapeForCamera(AABB &out_aabb, IConstBlockSource const &region, BlockPos const &pos) const;
    const AABB &getOutline(IConstBlockSource const &region, BlockPos const &pos, AABB &buffer) const;
    const AABB &getVisualShape(AABB &buffer) const;
    const AABB &getUIShape(AABB &buffer) const;
    bool getLiquidClipVolume(BlockSource &region, BlockPos const &pos, AABB &include_box) const;
    [[nodiscard]] bool requiresCorrectToolForDrops() const;
    [[nodiscard]] const Material &getMaterial() const;
    [[nodiscard]] float getThickness() const;
    bool getSecondPart(const BlockSource &region, const BlockPos &pos, BlockPos &out) const;
    [[nodiscard]] float getFriction() const;
    [[nodiscard]] float getDestroySpeed() const;
    [[nodiscard]] const HashedString &getName() const;
    [[nodiscard]] const CompoundTag &getSerializationId() const;
    [[nodiscard]] BlockRuntimeId getRuntimeId() const;
    [[nodiscard]] std::string toDebugString() const;
    [[nodiscard]] const BlockLegacy &getLegacyBlock() const;
    [[nodiscard]] const std::vector<HashedString> &getTags() const;
    [[nodiscard]] const BlockComponentDirectData &getDirectData() const;

    // Endstone begin
    template <typename T>
    T getState(const HashedString &name) const
    {
        return getLegacyBlock().getState<T>(name, data_);
    }
    // Endstone end

private:
    friend class ItemStackBase;
    friend class ItemDescriptor;

    BlockComponentStorage components_;                 // +8
    DataID data_;                                      // +112
    gsl::not_null<BlockLegacy *> legacy_block_;        // +120
    CachedComponentData cached_component_data_;        //
    BlockComponentDirectData direct_data_;             //
    std::vector<HashedString> tags_;                   //
    BlockSerializationId serialization_id_;            //
    HashType64 serialization_id_hash_;                 //
    std::uint32_t serialization_id_hash_for_network_;  //
    BlockRuntimeId network_id_;                        // +300
    bool has_runtime_id_;                              //
};
BEDROCK_STATIC_ASSERT_SIZE(Block, 312, 320);
