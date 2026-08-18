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
#include <string>
#include <unordered_map>

#include "bedrock/core/math/vec3.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/dataloadhelper/data_load_helper_type.h"
#include "bedrock/util/mirror.h"
#include "bedrock/util/rotation.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/internal_component_registry.h"
#include "bedrock/world/direction.h"
#include "bedrock/world/level/block_pos.h"

class DataLoadHelper {
public:
    virtual ~DataLoadHelper() = default;

    virtual Vec3 loadPosition(const Vec3 &position) = 0;
    virtual BlockPos loadBlockPosition(const BlockPos &block_pos) = 0;
    virtual BlockPos loadBlockPositionOffset(const BlockPos &block_pos_offset) = 0;
    virtual float loadRotationDegreesX(float value) = 0;
    virtual float loadRotationDegreesY(float value) = 0;
    virtual float loadRotationRadiansX(float value) = 0;
    virtual float loadRotationRadiansY(float value) = 0;
    virtual std::uint8_t loadFacingID(std::uint8_t facing) = 0;
    virtual Vec3 loadDirection(const Vec3 &direction) = 0;
    virtual Direction::Type loadDirection(Direction::Type direction) = 0;
    virtual Rotation loadRotation(Rotation rotation) = 0;
    virtual Mirror loadMirror(Mirror mirror) = 0;
    virtual ActorUniqueID loadActorUniqueID(ActorUniqueID id) = 0;
    virtual ActorUniqueID loadOwnerID(ActorUniqueID id) = 0;
    virtual const InternalComponentRegistry::ComponentInfo *loadActorInternalComponentInfo(
        const std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> &registry,
        const std::string &component_name) = 0;
    virtual DataLoadHelperType getType() const = 0;
    virtual bool shouldResetTime() = 0;
};
