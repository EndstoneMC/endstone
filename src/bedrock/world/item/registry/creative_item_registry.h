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

#include <functional>

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/world/item/registry/creative_group_info.h"
#include "bedrock/world/item/registry/creative_item_entry.h"

class CreativeItemRegistry : public Bedrock::EnableNonOwnerReferences {
public:
    CreativeGroupInfoPtr getCreativeGroup(std::uint32_t);
    [[nodiscard]] const std::vector<CreativeItemEntry> &getCreativeItemEntries() const;
    [[nodiscard]] const std::vector<CreativeGroupInfo> &getCreativeGroups() const;
    void forEachCreativeItemInstance(std::function<bool(const ItemInstance &)>) const;

private:
    std::vector<CreativeItemEntry> creative_items_;   // +24
    std::vector<CreativeGroupInfo> creative_groups_;  // +48
};
