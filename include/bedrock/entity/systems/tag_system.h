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
#include "bedrock/util/tag_registry.h"

namespace TagSystem {

template <typename TagID, typename TagSetID>
bool hasTag(TagsComponent<TagSetID> const &comp, const std::string &tag, TagRegistry<TagID, TagSetID> const &registry)
{
    auto tag_id = registry.tryGetTagID(tag);
    if (!tag_id.id) {
        return false;
    }
    return registry.tagContainedInSet(tag_id, comp.tag_set_id);
}

template <typename TagID, typename TagSetID>
bool removeTag(TagsComponent<TagSetID> &comp, std::string const &tag, TagRegistry<TagID, TagSetID> &registry)
{
    auto tag_id = registry.tryGetTagID(tag);
    if (tag_id.id) {
        comp.tag_set_id = registry.removeTagFromSet(tag_id, comp.tag_set_id);
    }
    return true;
}

}  // namespace TagSystem
