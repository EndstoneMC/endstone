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
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/util/bounds.h"
#include "bedrock/world/level/dimension/dimension_type.h"

class TickingAreasManager {
public:
    struct ScopedContext {
        mce::UUID uuid;
        std::string name;
        Bounds bounds;
        bool finished_loading;
        bool is_active;
    };

    ScopedContext addTransientScopedArea(DimensionType dimension, const std::string &name, std::uint64_t scope,
                                         mce::UUID uuid, const Bounds &bounds);
    void removeScopedAreas(const std::vector<mce::UUID> &uuids);
};
BEDROCK_STATIC_ASSERT_SIZE(TickingAreasManager::ScopedContext, 104, 96);
