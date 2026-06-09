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

#include "bedrock/dataloadhelper/default_data_load_helper.h"

Vec3 DefaultDataLoadHelper::loadPosition(Vec3 const &position)
{
    return position;
}

BlockPos DefaultDataLoadHelper::loadBlockPosition(BlockPos const &block_pos)
{
    return block_pos;
}

BlockPos DefaultDataLoadHelper::loadBlockPositionOffset(BlockPos const &block_pos_offset)
{
    return block_pos_offset;
}

float DefaultDataLoadHelper::loadRotationDegreesX(float x)
{
    return x;
}

float DefaultDataLoadHelper::loadRotationDegreesY(float y)
{
    return y;
}

float DefaultDataLoadHelper::loadRotationRadiansX(float x)
{
    return x;
}

float DefaultDataLoadHelper::loadRotationRadiansY(float y)
{
    return y;
}

std::uint8_t DefaultDataLoadHelper::loadFacingID(std::uint8_t facing)
{
    return facing;
}

Vec3 DefaultDataLoadHelper::loadDirection(Vec3 const &direction)
{
    return direction;
}

Direction::Type DefaultDataLoadHelper::loadDirection(Direction::Type direction)
{
    return direction;
}

Rotation DefaultDataLoadHelper::loadRotation(Rotation rotation_value)
{
    return rotation_value;
}

Mirror DefaultDataLoadHelper::loadMirror(Mirror mirror)
{
    return mirror;
}

ActorUniqueID DefaultDataLoadHelper::loadActorUniqueID(ActorUniqueID id)
{
    return id;
}

ActorUniqueID DefaultDataLoadHelper::loadOwnerID(ActorUniqueID id)
{
    return id;
}

InternalComponentRegistry::ComponentInfo const *DefaultDataLoadHelper::loadActorInternalComponentInfo(
    std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> const &registry,
    std::string const &component_name)
{
    auto it = registry.find(component_name);
    if (it != registry.end()) {
        return &it->second;
    }
    return nullptr;
}

DataLoadHelperType DefaultDataLoadHelper::getType() const
{
    return DataLoadHelperType::Default;
}

bool DefaultDataLoadHelper::shouldResetTime()
{
    return false;
}
