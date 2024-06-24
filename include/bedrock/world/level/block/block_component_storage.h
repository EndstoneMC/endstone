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
#include <memory>
#include <vector>

#include "bedrock/core/type_id.h"
#include "bedrock/world/level/block/components/block_component_base.h"

class BlockComponentStorage {
public:
    virtual ~BlockComponentStorage() = 0;

private:
    std::vector<std::pair<Bedrock::typeid_t<void>, std::unique_ptr<BlockComponentBase>>> components_;  // +8
    bool allow_adding_;                                                                                // +32
    bool allow_replacement_;                                                                           // +33
    bool allow_try_get_before_finalization_;                                                           // +34
};
BEDROCK_STATIC_ASSERT_SIZE(BlockComponentStorage, 40, 40);
