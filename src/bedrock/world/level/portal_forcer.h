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

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/util/random.h"
#include "bedrock/world/level/block/portal_axis.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/portal_record.h"
#include "bedrock/world/level/saveddata/saved_data.h"

class Actor;
class BaseGameVersion;
class BlockSource;
class CompoundTag;
class Level;
class PortalShape;
struct DimensionTransitionComponent;

class PortalForcer : public SavedData {
public:
    static const std::string PORTAL_FILE_ID;
    static const int PORTAL_CREATION_RADIUS = 16;
    static const int PORTAL_SEARCH_RADIUS = 128;
    static const BaseGameVersion MIN_PORTAL_REPLACE_BLOCK_FIX_VERSION;
    PortalForcer(Level &level);
    void force(Actor &entity, const DimensionTransitionComponent &portal_entrance_component);
    bool findPortal(DimensionType dimension_id, const BlockPos &center_block_pos, const int radius,
                    BlockPos &target_block_pos) const;
    const PortalRecord &createPortal(const Actor &entity, const int radius);
    [[nodiscard]] bool portalRecordExists(DimensionType dimension_type, const PortalRecord &record) const;
    const PortalRecord &addPortalRecord(DimensionType dimension_type, const PortalShape &shape);
    const PortalRecord &addPortalRecord(DimensionType dimension_type, PortalRecord to_add);
    void removePortalRecord(BlockSource &source, const BlockPos &pos);
    void printPortalRecords(DimensionType) const;
    void removeMisalignedPortalRecords(BlockSource &region, const PortalShape &reference_shape,
                                       const PortalRecord &reference_record);
    void deserialize(const CompoundTag &tag) override;
    void serialize(CompoundTag &tag) const override;
    static float _getNormalizedPortalPosition(const Vec3 &enter_target, const PortalRecord &enter_portal_record);
    static void _applyNormalizedPortalPosition(Vec3 &target_position, PortalAxis &target_portal_axis,
                                               const float normalized_enter_position,
                                               const PortalRecord &portal_record);
    static bool canPortalReplaceBlock(BlockSource &region, const BlockPos &block_pos);
    [[nodiscard]] std::optional<PortalRecord> _findPortal(
        DimensionType dimension_id, const BlockPos &center_block_pos, const int radius,
        BlockPos &target_block_pos) const;  // Endstone: private -> public

private:
    void _travelPortal(Actor &entity, const Vec3 &target_block_pos, const PortalAxis &target_portal_axis,
                       const PortalAxis &original_portal_axis, DimensionType dimension_id) const;
    using PortalRecordSet = std::unordered_set<PortalRecord>;
    Level &level_;
    Random random_;
    std::unordered_map<DimensionType, std::unordered_set<PortalRecord>> portal_records_;
};
BEDROCK_STATIC_ASSERT_SIZE(PortalForcer, 2680, 2648);
