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

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/phys/aabb.h"

struct StateVectorComponent {
    Vec3 position;        // +0
    Vec3 position_prev;   // +12
    Vec3 position_delta;  // +24
};

struct AABBShapeComponent {
    AABB aabb;
};

struct ActorRotationComponent {
    Vec2 rotation;
    Vec2 rotation_prev;
};

struct ActorWalkAnimationComponent {};

struct BuiltInActorComponents {
    StateVectorComponent *state_vector;
    AABBShapeComponent *aabb;
    ActorRotationComponent *rotation;
    ActorWalkAnimationComponent *walk_animation;
};
BEDROCK_STATIC_ASSERT_SIZE(BuiltInActorComponents, 32, 32);
