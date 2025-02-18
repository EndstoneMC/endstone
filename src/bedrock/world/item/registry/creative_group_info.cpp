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

#include "bedrock/world/item/registry/creative_group_info.h"

const std::string &CreativeGroupInfo::getName() const
{
    return name_.getString();
}

const ItemInstance &CreativeGroupInfo::getIcon() const
{
    return icon_;
}

CreativeItemCategory CreativeGroupInfo::getCreativeCategory() const
{
    return category_;
}

std::uint32_t CreativeGroupInfo::getIndex() const
{
    return index_;
}

const std::vector<unsigned int> &CreativeGroupInfo::getItems() const
{
    return item_indexes_;
}
