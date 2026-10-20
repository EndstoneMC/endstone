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

#include "bedrock/world/item/item_instance.h"

const ItemInstance ItemInstance::EMPTY_ITEM{};

ItemInstance::ItemInstance(const ItemStackBase &item) : ItemStackBase(item) {}

ItemInstance::ItemInstance(std::string_view name, int count, int aux_value, const CompoundTag *user_data)
    : ItemStackBase(name, count, aux_value, user_data)
{
}

ItemInstance::ItemInstance(const Item &item, int count, int aux_value, const CompoundTag *user_data)
    : ItemStackBase(item, count, aux_value, user_data)
{
}

void ItemInstance::reinit(const Item &item, int count, int aux_value)
{
    ItemStackBase::operator=(ItemInstance(item, count, aux_value, nullptr));
}

void ItemInstance::reinit(std::string_view name, int count, int aux_value)
{
    ItemStackBase::operator=(ItemInstance(name, count, aux_value, nullptr));
}

ItemInstance ItemInstance::fromTag(const CompoundTag &tag)
{
    return BEDROCK_CALL(&ItemInstance::fromTag, tag);
}
