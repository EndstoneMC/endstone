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

class BlockComponentStorage {
public:
    struct ComponentBase {
        virtual ~ComponentBase();
    };

private:
    std::size_t padding_[12];                            // +0
    bool allow_adding_components_;                       // +96
    bool allow_replacement_components_;                  // +97
    bool allow_try_get_components_before_finalization_;  // +98
};
BEDROCK_STATIC_ASSERT_SIZE(BlockComponentStorage, 104, 104);
