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
#include "bedrock/util/index_set.h"

template <typename TagID, typename TagSetID>
class TagRegistry {
public:
    std::vector<std::string> getTagsInSet(TagSetID tag_set_id)
    {
        std::vector<std::string> tags;
        auto index_set = sets_[tag_set_id.id.value()];
        for (const auto index : index_set.getPacked()) {
            tags.emplace_back(tags_[index]);
        }
        return tags;
    }

private:
    std::unordered_map<HashedString, std::uint64_t> tag_index_map_;  // +0
    std::vector<std::string> tags_;                                  // +64
    std::vector<IndexSet> sets_;                                     // +88
    TagSetID empty_tag_set_;                                         // +136
};
