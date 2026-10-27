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
#include <memory>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/platform/brstd/function_ref.h"
#include "bedrock/shared_ptr.h"
#include "bedrock/world/level/block/block_type.h"

class BlockTypeRegistry {
public:
    void forEachBlockType(brstd::function_ref<bool(BlockType const &)> callback) const;

private:
    std::map<HashedString, std::unique_ptr<BlockType>> block_lookup_map_;
};
