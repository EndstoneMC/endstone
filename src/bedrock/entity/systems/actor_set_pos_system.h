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
#include "bedrock/core/math/vec2.h"
#include "bedrock/entity/components/hitbox_component.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"

class ActorSetPosSystem {
public:
    static void moveHitboxTo(const Vec3 &pos, std::vector<Hitbox> &hitboxes);
    static AABB refreshAABB(float offset, const Vec3 &position, const Vec2 &bb_dim);
    static void setPosition(EntityContext &entity, const Vec3 &position, bool is_client_side);
};
