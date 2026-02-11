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
#include "bedrock/world/level/block/block_serialization_id.h"
#include "bedrock/world/level/block/block_type.h"
#include "bedrock/world/level/block/components/block_component_direct_data.h"
#include "bedrock/world/level/block/components/block_component_storage.h"
#include "bedrock/world/level/block/states/block_state_registry.h"

enum class BlockOcclusionType : int {
    Unknown = 0,
    HalfSlab = 1,
    Leaf = 2,
    Connects2D = 3,
    isLiquid = 4,
    Portal = 5,
    Ice = 6,
    Cactus = 7,
    PowderSnow = 8,
    SculkShrieker = 9,
    Default = 10,
};

struct CachedComponentData {
    Brightness emissive_brightness;     // +0
    bool is_solid;                      // +1
    BlockOcclusionType occlusion_type;  // +4
};

class Block {
public:
    virtual ~Block() = default;

    [[nodiscard]] bool hasProperty(BlockProperty) const;
    [[nodiscard]] Brightness getLightEmission() const;
    [[nodiscard]] float getTranslucency() const;
    [[nodiscard]] bool isSolid() const;
    [[nodiscard]] Brightness getLight() const;
    [[nodiscard]] int getFlameOdds() const;
    [[nodiscard]] int getBurnOdds() const;
    [[nodiscard]] float getExplosionResistance() const;
    [[nodiscard]] bool hasState(const BlockState &) const;
    [[nodiscard]] bool hasState(const HashedString &) const;
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
    [[nodiscard]] bool isLavaBlocking() const;
    [[nodiscard]] bool requiresCorrectToolForDrops() const;
    [[nodiscard]] float getThickness() const;
    bool getSecondPart(const BlockSource &region, const BlockPos &pos, BlockPos &out) const;
    void destroy(BlockSource &region, const BlockPos &pos, Actor *entity_source) const;
    [[nodiscard]] const Material &getMaterial() const;
    [[nodiscard]] float getFriction() const;
    [[nodiscard]] float getDestroySpeed() const;
    [[nodiscard]] const HashedString &getName() const;
    [[nodiscard]] const CompoundTag &getSerializationId() const;
    [[nodiscard]] BlockRuntimeId getRuntimeId() const;
    [[nodiscard]] std::string toDebugString() const;
    [[nodiscard]] const BlockType &getBlockType() const;
    [[nodiscard]] const std::vector<HashedString> &getTags() const;
    [[nodiscard]] const BlockComponentDirectData &getDirectData() const;

    // Endstone begin
    template <typename T>
    T getState(const HashedString &name) const
    {
        const auto *state = BlockStateRegistry::get().getState(name);
        if (!state) {
            T default_value{};
            return default_value;
        }
        return getBlockType().getState<T>(*state, data_);
    }

    template <typename T>
    const Block *trySetState(const HashedString &name, T val) const
    {
        const auto *state = BlockStateRegistry::get().getState(name);
        if (!state) {
            return nullptr;
        }
        return getBlockType().trySetState<T>(*state, val, data_);
    }

    template <typename T>
    gsl::strict_not_null<const Block *> setState(const HashedString &name, T val) const
    {
        return gsl::make_strict_not_null(trySetState(name, val));
    }
    // Endstone end

private:
    friend class ItemStackBase;
    friend class ItemDescriptor;

    BlockComponentStorage components_;                 // +8
    DataID data_;                                      // +112
    gsl::not_null<BlockType *> block_type_;            // +120
    CachedComponentData cached_component_data_;        //
    BlockComponentDirectData direct_data_;             //
    std::vector<HashedString> tags_;                   //
    BlockSerializationId serialization_id_;            //
    HashType64 serialization_id_hash_;                 //
    std::uint32_t serialization_id_hash_for_network_;  //
    BlockRuntimeId network_id_;                        // +300
    bool has_runtime_id_;                              //
};
BEDROCK_STATIC_ASSERT_SIZE(Block, 280, 288);
