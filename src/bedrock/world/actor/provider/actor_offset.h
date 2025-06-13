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
#include "bedrock/entity/components/offsets_component.h"
#include "bedrock/world/actor/provider/actor_entity_access.h"

namespace ActorOffset {
void initializeActor(EntityContext &);
void initializePlayer(EntityContext &);
float getHeightOffset(const EntityContext &);
void setHeightOffset(EntityContext &entity, float height_offset)
{
    ActorEntityAccess::getPersistentComponent<OffsetsComponent>(entity)->height_offset = height_offset;
}
void setExplosionOffset(EntityContext &, float);
Vec3 getHeadOffset(const EntityContext &);
Vec3 getEyeOffset(const EntityContext &);
void setEyeOffset(EntityContext &, const Vec3 &);
void setBreathingOffset(EntityContext &, const Vec3 &);
}  // namespace ActorOffset
