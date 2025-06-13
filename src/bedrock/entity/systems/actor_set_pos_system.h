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

#include "bedrock/core/math/vec3.h"
#include "bedrock/entity/components/hitbox_component.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"

class ActorSetPosSystem {
public:
    static void moveHitboxTo(const Vec3 &pos, std::vector<Hitbox> &hitboxes)
    {
        for (auto &hitbox : hitboxes) {
            Vec3 half;
            half.x = (hitbox.aabb.max.x - hitbox.aabb.min.x) * 0.5F;
            half.y = (hitbox.aabb.max.y - hitbox.aabb.min.y) * 0.5F;
            half.z = (hitbox.aabb.max.z - hitbox.aabb.min.z) * 0.5F;

            Vec3 centered_min = {pos.x - half.x, pos.y - half.y, pos.z - half.z};
            Vec3 centered_max = {pos.x + half.x, pos.y + half.y, pos.z + half.z};

            hitbox.aabb.min.x = centered_min.x + hitbox.pivot.x;
            hitbox.aabb.min.y = centered_min.y + hitbox.pivot.y;
            hitbox.aabb.min.z = centered_min.z + hitbox.pivot.z;
            hitbox.aabb.max.x = centered_max.x + hitbox.pivot.x;
            hitbox.aabb.max.y = centered_max.y + hitbox.pivot.y;
            hitbox.aabb.max.z = centered_max.z + hitbox.pivot.z;
        }
    }

    static AABB refreshAABB(float offset, const Vec3 &position, const Vec2 &bb_dim)
    {
        return {position.x - (0.5F * bb_dim.x), position.y - offset,
                position.z - (0.5F * bb_dim.x), position.x + (0.5F * bb_dim.x),
                position.y - offset + bb_dim.y, position.z + (0.5F * bb_dim.x)};
    }

    static void setPosition(EntityContext &entity, const Vec3 &position, bool is_client_side);
};
