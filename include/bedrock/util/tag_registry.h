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

#include "bedrock/forward.h"
#include "bedrock/util/id_type.h"

template <typename TagIDType, typename TagSetIDType>
class TagRegistry {
    std::unordered_map<HashedString, std::uint64_t> tag_index_map_;
    std::vector<std::string> tags_;
    std::vector<IndexSet> sets_;
    IDType<TagSetIDType> empty_tag_set_;
};
