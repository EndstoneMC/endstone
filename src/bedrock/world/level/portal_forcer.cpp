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

#include "bedrock/world/level/portal_forcer.h"

#include <cstdint>
#include <cstdlib>

float PortalForcer::_getNormalizedPortalPosition(const Vec3 &enter_target, const PortalRecord &enter_portal_record)
{
    if (enter_portal_record.x_inc != 0) {
        return (enter_target.x - static_cast<float>(enter_portal_record.base_block_pos.x)) /
               static_cast<float>(enter_portal_record.span);
    }
    if (enter_portal_record.z_inc != 0) {
        return (enter_target.z - static_cast<float>(enter_portal_record.base_block_pos.z)) /
               static_cast<float>(enter_portal_record.span);
    }
    return 0.0F;
}

void PortalForcer::_applyNormalizedPortalPosition(Vec3 &target_position, PortalAxis &target_portal_axis,
                                                  const float normalized_enter_position,
                                                  const PortalRecord &portal_record)
{
    target_position = portal_record.base_block_pos;
    if (portal_record.x_inc != 0) {
        target_position.x += static_cast<float>(portal_record.span) * normalized_enter_position;
        target_position.z += 0.5F;
        target_portal_axis = PortalAxis::X;
    }
    if (portal_record.z_inc != 0) {
        target_position.x += 0.5F;
        target_position.z += static_cast<float>(portal_record.span) * normalized_enter_position;
        target_portal_axis = PortalAxis::Z;
    }
}

std::optional<PortalRecord> PortalForcer::_findPortal(DimensionType dimension_id, const BlockPos &center_block_pos,
                                                      const int radius, BlockPos &target_block_pos) const
{
    target_block_pos = BlockPos::ZERO;
    std::optional<PortalRecord> result;
    const auto it = portal_records_.find(dimension_id);
    if (it == portal_records_.end()) {
        return result;
    }
    auto closest_distance = -1.0F;
    for (const auto &record : it->second) {
        for (auto i = 0; i < record.span; ++i) {
            const BlockPos block_pos(record.base_block_pos.x + (i * record.x_inc), record.base_block_pos.y,
                                     record.base_block_pos.z + (i * record.z_inc));
            if (std::abs(center_block_pos.x - block_pos.x) > radius ||
                std::abs(center_block_pos.z - block_pos.z) > radius) {
                continue;
            }
            const auto dx = static_cast<std::int64_t>(block_pos.x) - center_block_pos.x;
            const auto dy = static_cast<std::int64_t>(block_pos.y) - center_block_pos.y;
            const auto dz = static_cast<std::int64_t>(block_pos.z) - center_block_pos.z;
            const auto distance = static_cast<float>((dx * dx) + (dy * dy) + (dz * dz));
            if (closest_distance < 0.0F || closest_distance > distance) {
                target_block_pos = block_pos;
                result = record;
                closest_distance = distance;
            }
        }
    }
    return result;
}
