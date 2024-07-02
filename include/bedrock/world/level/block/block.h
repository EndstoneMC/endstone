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
#include "bedrock/core/hashed_string.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/level/block/block_component_storage.h"
#include "bedrock/world/level/block/block_legacy.h"
#include "bedrock/world/level/block/cached_component_data.h"

using BlockRuntimeId = std::uint32_t;

class Block : public BlockComponentStorage {
public:
    [[nodiscard]] bool canDropWithAnyTool() const
    {
        return legacy_block_->canDropWithAnyTool();
    }

    [[nodiscard]] const CompoundTag &getSerializationId() const
    {
        return serialization_id_;
    }

    [[nodiscard]] BlockRuntimeId getRuntimeId() const
    {
        return runtime_id_;
    }

    [[nodiscard]] BurnOdds getBurnOdds() const
    {
        return cached_component_data_.burn_odds;
    }

    [[nodiscard]] float getDestroySpeed() const
    {
        return cached_component_data_.destroy_speed;
    }

    [[nodiscard]] float getExplosionResistance() const
    {
        return cached_component_data_.explosion_resistance;
    }

    [[nodiscard]] FlameOdds getFlameOdds() const
    {
        return cached_component_data_.flame_odds;
    }

    [[nodiscard]] float getFriction() const
    {
        return cached_component_data_.friction;
    }

    [[nodiscard]] Brightness getLight() const
    {
        return cached_component_data_.light;
    }

    [[nodiscard]] Brightness getLightEmission() const
    {
        return cached_component_data_.light_emission;
    }

    [[nodiscard]] const Material &getMaterial() const
    {
        return legacy_block_->getMaterial();
    }

    [[nodiscard]] const std::vector<HashedString> &getTags() const
    {
        if (!tags_.empty()) {
            return tags_;
        }
        return legacy_block_->getTags();
    }

    [[nodiscard]] float getThickness() const
    {
        return legacy_block_->getThickness();
    }

    [[nodiscard]] const BlockLegacy &getLegacyBlock() const
    {
        return *legacy_block_;
    }

    bool getCollisionShape(AABB &out_aabb, IConstBlockSource const &region, BlockPos const &pos,
                           optional_ref<GetCollisionShapeInterface const> entity) const
    {
        out_aabb = legacy_block_->getCollisionShape(*this, region, pos, entity);
        return out_aabb.min.x < out_aabb.max.x && out_aabb.min.y < out_aabb.max.y && out_aabb.min.z < out_aabb.max.z;
    }

private:
    friend class ItemStackBase;

    DataID data_;                                          // +40 (+36)
    gsl::not_null<BlockLegacy *> legacy_block_;            // +48 (+40)
    CachedComponentData cached_component_data_;            // +56 (+48)
    std::vector<HashedString> tags_;                       // +136 (+128)
    CompoundTag serialization_id_;                         // +160 (+152)
    std::size_t serialization_id_hash_;                    // +184
    std::uint32_t raw_serialization_id_hash_for_network_;  // +192
    BlockRuntimeId runtime_id_;                            // +196
    bool unknown4_;                                        // +200
};
BEDROCK_STATIC_ASSERT_SIZE(Block, 208, 208);
