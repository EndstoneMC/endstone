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

#include "bedrock/world/level/block/bed_block.h"

#include "bedrock/world/direction.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/block/registry/block_type_registry.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/material/material_type.h"

std::optional<BlockPos> BedBlock::findWakeupPosition(BlockSource &region, const BlockPos &pos,
                                                     const std::optional<Vec3> &entered_bed_pos)
{
    const auto &block = region.getBlock(pos);

    if (block.getName() != VanillaBlockTypeIds::Bed) {
        return std::nullopt;
    }

    // Determine bed orientation
    const auto direction = block.getState<int>(VanillaStateIds::Direction);

    // Find the head piece position
    auto head_of_bed_pos = pos;
    if (!block.hasState(VanillaStateIds::HeadPieceBit)) {
        block.getSecondPart(region, pos, head_of_bed_pos);
    }

    const auto &rotation = BedBlockSpawnOffset::RESPAWN_OFFSET_ROTATION_FROM_DIRECTION[direction];

    // Decide which side of the bed the player entered from
    bool use_right_offset = true;
    if (entered_bed_pos) {
        const auto &entered_pos = entered_bed_pos.value();
        switch (direction) {
        case Direction::SOUTH:
            use_right_offset = (static_cast<float>(head_of_bed_pos.x) + 0.5F) >= entered_pos.x;
            break;
        case Direction::WEST:
            use_right_offset = (static_cast<float>(head_of_bed_pos.z) + 0.5F) >= entered_pos.z;
            break;
        case Direction::NORTH:
            use_right_offset = (static_cast<float>(head_of_bed_pos.x) + 0.5F) <= entered_pos.x;
            break;
        case Direction::EAST:
            use_right_offset = (static_cast<float>(head_of_bed_pos.z) + 0.5F) <= entered_pos.z;
            break;
        default:
            break;
        }
    }

    const auto &offsets = use_right_offset ? BedBlockSpawnOffset::RESPAWN_OFFSETS_NORTH_RIGHT
                                           : BedBlockSpawnOffset::RESPAWN_OFFSETS_NORTH_LEFT;

    std::optional<BlockPos> first_valid_standup_pos;
    for (const auto &offset : offsets) {
        // Rotate and translate offset
        auto relative = offset.transform(rotation, Mirror::None, Vec3::ZERO);
        auto standup_position = head_of_bed_pos + relative;

        // Check for a valid stand-up position
        if (isValidStandUpPosition(region, standup_position)) {
            // Prefer non-dangerous positions
            if (!isDangerousSpawnPosition(region, standup_position)) {
                return standup_position;
            }
            // Remember the first valid but dangerous position as fallback
            if (!first_valid_standup_pos) {
                first_valid_standup_pos = standup_position;
            }
        }
    }

    return first_valid_standup_pos;
}

bool BedBlock::isDangerousSpawnPosition(BlockSource &region, const BlockPos &pos)
{
    const auto &block = region.getBlock(pos);
    if (block.hasProperty(BlockProperty::CausesDamage) || block.getMaterial().isType(MaterialType::Lava) ||
        block.getName() == VanillaBlockTypeIds::WitherRose) {
        return true;
    }

    const auto &block_below = region.getBlock(pos.below());
    if (block_below.hasProperty(BlockProperty::CausesDamage) || block_below.getMaterial().isType(MaterialType::Lava) ||
        block_below.getName() == VanillaBlockTypeIds::WitherRose) {
        return true;
    }

    return false;
}

bool BedBlock::isValidStandUpPosition(BlockSource &region, const BlockPos &pos)
{
    const auto &block = region.getBlock(pos);
    const auto is_bed = block.getName() == VanillaBlockTypeIds::Bed;
    const auto &block_below = region.getBlock(pos.below());
    if (AABB aabb; !block_below.getCollisionShape(aabb, region, pos, nullptr)) {
        return false;
    }

    if (!is_bed && (region.isSolidBlockingBlock(pos) || block.getName() == VanillaBlockTypeIds::EndPortal ||
                    block.getName() == VanillaBlockTypeIds::EndPortalFrame)) {
        return false;
    }

    // Check that one block above is air
    if (!region.isEmptyBlock(pos.above())) {
        return false;
    }

    // If standing on a bed, ensure two blocks of clearance
    if (is_bed && !region.isEmptyBlock(pos.above(2))) {
        return false;
    }

    return true;
}
