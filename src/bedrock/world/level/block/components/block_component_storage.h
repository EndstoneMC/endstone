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

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/type_id.h"
#include "bedrock/platform/brstd/flat_map.h"
#include "bedrock/platform/brstd/flat_set.h"

class BlockComponentStorage {
public:
    struct ComponentBase {
        virtual ~ComponentBase();
    };

private:
    brstd::flat_map<Bedrock::typeid_t<void>, std::unique_ptr<ComponentBase>> components_;
    brstd::flat_set<Bedrock::typeid_t<void>> stateless_components_;
    bool allow_adding_components_;  // +72
    bool allow_replacement_components_;
    bool allow_try_get_components_before_finalization_;
};
BEDROCK_STATIC_ASSERT_SIZE(BlockComponentStorage, 80, 80);
