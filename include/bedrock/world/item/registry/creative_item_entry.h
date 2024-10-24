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

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"
#include "bedrock/world/item/item_instance.h"

class CreativeItemRegistry;

struct CreativeItemNetIdTag {};
using CreativeItemNetId = TypedServerNetId<CreativeItemNetIdTag, unsigned int>;

class CreativeItemEntry : public Bedrock::EnableNonOwnerReferences {
public:
    [[nodiscard]] ItemInstance const &getItemInstance() const;

private:
    CreativeItemRegistry *registry_;  // +24
    std::uint32_t group_index_;
    CreativeItemNetId creative_net_id_;
    ItemInstance item_instance_;
    std::uint32_t index;
};
