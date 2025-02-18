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

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/world/item/item_category.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/registry/creative_item_entry_fwd.h"

class CreativeGroupInfo : public Bedrock::EnableNonOwnerReferences {
public:
    CreativeGroupInfo(CreativeItemRegistryPtr, CreativeItemCategory, const HashedString &, uint32_t,
                      const ItemInstance &);
    CreativeGroupInfo(CreativeItemRegistryPtr, CreativeItemCategory, HashedString &&, uint32_t, ItemInstance &&);
    CreativeGroupInfo(CreativeItemRegistryPtr, CreativeItemCategory, std::uint32_t);

    CreativeItemGroupCategoryPtr getGroupCategory();
    const std::string &getName() const;
    const ItemInstance &getIcon() const;
    CreativeItemCategory getCreativeCategory() const;
    std::uint32_t getIndex() const;
    const std::vector<unsigned int> &getItems() const;

private:
    CreativeItemCategory category_;           // +24
    CreativeItemRegistryPtr registry_;        // +32
    HashedString name_;                       // +40
    ItemInstance icon_;                       // +80
    std::uint32_t index_;                     // +208
    std::vector<unsigned int> item_indexes_;  // +216
};
