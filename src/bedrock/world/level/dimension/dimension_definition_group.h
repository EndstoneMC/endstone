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

#include <map>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/generator_type.h"

class DimensionDefinitionGroup {
public:
    struct DimensionDefinition {
        int min_y;
        int height_range;
        GeneratorType generator_type;
        DimensionType dimension_type;
        mce::UUID pack_id;
        std::string default_biome;
    };

    [[nodiscard]] const std::map<std::string, DimensionDefinition> &getDefinitions() const
    {
        return dimension_definitions_;
    }

private:
    std::map<std::string, DimensionDefinition> dimension_definitions_;
};
BEDROCK_STATIC_ASSERT_SIZE(DimensionDefinitionGroup::DimensionDefinition, 64, 56);
BEDROCK_STATIC_ASSERT_SIZE(DimensionDefinitionGroup, 16, 24);
