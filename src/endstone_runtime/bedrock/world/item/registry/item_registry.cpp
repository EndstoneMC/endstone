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

#include "bedrock/world/item/registry/item_registry.h"

WeakPtr<Item> ItemRegistry::getItem(int id) const
{
    return id_to_item_map_.at(id);
}

WeakPtr<Item> ItemRegistry::getItem(const HashedString &name) const
{
    return name_to_item_map_.at(name);
}

Bedrock::NonOwnerPointer<CreativeItemRegistry> ItemRegistry::getCreativeItemRegistry() const
{
    return Bedrock::NonOwnerPointer<CreativeItemRegistry>(*creative_item_registry_);
}
