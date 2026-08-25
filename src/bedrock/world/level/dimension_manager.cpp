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

std::optional<DimensionType> DimensionManager::serverRegisterCustomDimension(std::string_view name,
                                                                             const mce::UUID &pack_id)
{
    return BEDROCK_CALL(&DimensionManager::serverRegisterCustomDimension, this, name, pack_id);
}

WeakRef<Dimension> DimensionManager::getOrCreateDimension(std::string_view name)
{
    return BEDROCK_CALL(static_cast<WeakRef<Dimension> (DimensionManager::*)(std::string_view)>(
                            &DimensionManager::getOrCreateDimension),
                        this, name);
}
