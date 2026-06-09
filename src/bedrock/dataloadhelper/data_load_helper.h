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

    virtual Vec3 loadPosition(Vec3 const &position) = 0;
    virtual BlockPos loadBlockPosition(BlockPos const &block_pos) = 0;
    virtual BlockPos loadBlockPositionOffset(BlockPos const &block_pos_offset) = 0;
    virtual float loadRotationDegreesX(float x) = 0;
    virtual float loadRotationDegreesY(float y) = 0;
    virtual float loadRotationRadiansX(float x) = 0;
    virtual float loadRotationRadiansY(float y) = 0;
    virtual std::uint8_t loadFacingID(std::uint8_t facing) = 0;
    virtual Vec3 loadDirection(Vec3 const &direction) = 0;
    virtual Direction::Type loadDirection(Direction::Type direction) = 0;
    virtual Rotation loadRotation(Rotation rotation) = 0;
    virtual Mirror loadMirror(Mirror mirror) = 0;
    virtual ActorUniqueID loadActorUniqueID(ActorUniqueID id) = 0;
    virtual ActorUniqueID loadOwnerID(ActorUniqueID id) = 0;
    virtual InternalComponentRegistry::ComponentInfo const *loadActorInternalComponentInfo(
        std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> const &registry,
        std::string const &component_name) = 0;
    [[nodiscard]] virtual DataLoadHelperType getType() const = 0;
    virtual bool shouldResetTime() = 0;
};
