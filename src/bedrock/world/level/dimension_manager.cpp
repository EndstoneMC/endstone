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

#include "bedrock/world/level/dimension_manager.h"

#include "bedrock/symbol.h"

std::optional<DimensionType> DimensionManager::serverRegisterCustomDimension(
    std::string_view name, DimensionDefinitionGroup::DimensionDefinition &json_def)
{
    if (dimension_name_id_store_->contains(name)) {
        return std::nullopt;
    }
    const DimensionType type{dimension_name_id_store_->getOrCreateId(name).value};
    json_def.dimension_type = type;
    if (_registerDimensionWithDimensionDefinitionGroup(name, json_def)) {
        _registerCustomDimensionWithFactory(name, type);
    }
    return type;
}

bool DimensionManager::_registerDimensionWithDimensionDefinitionGroup(
    std::string_view name, const DimensionDefinitionGroup::DimensionDefinition &json_def)
{
    return BEDROCK_CALL(&DimensionManager::_registerDimensionWithDimensionDefinitionGroup, this, name, json_def);
}

void DimensionManager::_registerCustomDimensionWithFactory(std::string_view name, const DimensionType type)
{
    BEDROCK_CALL(&DimensionManager::_registerCustomDimensionWithFactory, this, name, type);
}

WeakRef<Dimension> DimensionManager::getOrCreateDimension(std::string_view name)
{
    return BEDROCK_CALL(static_cast<WeakRef<Dimension> (DimensionManager::*)(std::string_view)>(
                            &DimensionManager::getOrCreateDimension),
                        this, name);
}
