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
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"

class IWorldRegistriesProvider {
public:
    virtual ~IWorldRegistriesProvider() = 0;
    [[nodiscard]] virtual const BiomeRegistry &getBiomeRegistry() const = 0;
    virtual BiomeRegistry &getBiomeRegistry() = 0;
    [[nodiscard]] virtual const BlockPalette &getBlockPalette() const = 0;
    virtual BlockPalette &getBlockPalette() = 0;
    [[nodiscard]] virtual const FeatureRegistry &getFeatureRegistry() const = 0;
    virtual FeatureRegistry &getFeatureRegistry() = 0;
    [[nodiscard]] virtual const FeatureTypeFactory &getFeatureTypeFactory() const = 0;
    virtual FeatureTypeFactory &getFeatureTypeFactory() = 0;
    [[nodiscard]] virtual const JigsawStructureRegistry &getJigsawStructureRegistry() const = 0;
    virtual JigsawStructureRegistry &getJigsawStructureRegistry() = 0;
    [[nodiscard]] virtual const StructureSpawnRegistry &getStructureSpawnRegistry() const = 0;
    virtual StructureSpawnRegistry &getStructureSpawnRegistry() = 0;
    [[nodiscard]] virtual const Bedrock::NotNullNonOwnerPtr<StructureManager> getStructureManager() const = 0;
    virtual Bedrock::NotNullNonOwnerPtr<StructureManager> getStructureManager() = 0;
    [[nodiscard]] virtual const BiomeComponentFactory &getBiomeComponentFactory() const = 0;
    virtual BiomeComponentFactory &getBiomeComponentFactory() = 0;
    [[nodiscard]] virtual const SurfaceBuilderRegistry &getSurfaceBuilderRegistry() const = 0;
    virtual SurfaceBuilderRegistry &getSurfaceBuilderRegistry() = 0;
    [[nodiscard]] virtual const DimensionOwnerFactory &getDimensionFactory() const = 0;
    virtual DimensionOwnerFactory &getDimensionFactory() = 0;
    [[nodiscard]] virtual ActorDefinitionGroup *getEntityDefinitions() const = 0;
};
