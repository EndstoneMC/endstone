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

#include "bedrock/dataloadhelper/data_load_helper.h"

class DefaultDataLoadHelper : public DataLoadHelper {
public:
    Vec3 loadPosition(Vec3 const &position) override;
    BlockPos loadBlockPosition(BlockPos const &block_pos) override;
    BlockPos loadBlockPositionOffset(BlockPos const &block_pos_offset) override;
    float loadRotationDegreesX(float x) override;
    float loadRotationDegreesY(float y) override;
    float loadRotationRadiansX(float x) override;
    float loadRotationRadiansY(float y) override;
    std::uint8_t loadFacingID(std::uint8_t facing) override;
    Vec3 loadDirection(Vec3 const &direction) override;
    Direction::Type loadDirection(Direction::Type direction) override;
    Rotation loadRotation(Rotation rotation) override;
    Mirror loadMirror(Mirror mirror) override;
    ActorUniqueID loadActorUniqueID(ActorUniqueID id) override;
    ActorUniqueID loadOwnerID(ActorUniqueID id) override;
    InternalComponentRegistry::ComponentInfo const *loadActorInternalComponentInfo(
        std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> const &registry,
        std::string const &component_name) override;
    [[nodiscard]] DataLoadHelperType getType() const override;
    bool shouldResetTime() override;
};
