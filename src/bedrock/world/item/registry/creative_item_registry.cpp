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

#include "bedrock/world/item/registry/creative_item_registry.h"

#include "bedrock/world/item/item_instance.h"

CreativeGroupInfoPtr CreativeItemRegistry::getCreativeGroup(const uint32_t index)
{
    return &creative_groups_[index];
}

const std::vector<CreativeItemEntry> &CreativeItemRegistry::getCreativeItemEntries() const
{
    return creative_items_;
}

const std::vector<CreativeGroupInfo> &CreativeItemRegistry::getCreativeGroups() const
{
    return creative_groups_;
}

void CreativeItemRegistry::forEachCreativeItemInstance(std::function<bool(const ItemInstance &)> func) const
{
    for (const auto &creative_item : creative_items_) {
        if (!func(creative_item.getItemInstance())) {
            break;
        }
    }
}
