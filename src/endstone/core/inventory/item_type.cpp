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

#include "endstone/core/inventory/item_type.h"

namespace endstone::core {

EndstoneItemType::EndstoneItemType(const ::Item &item) : item_(item) {}

std::string_view EndstoneItemType::getId() const
{
    return item_.getFullItemName();
}

std::string EndstoneItemType::getTranslationKey() const
{
    return item_.buildDescriptionId(ItemDescriptor(item_, 0), nullptr);
}

int EndstoneItemType::getMaxStackSize() const
{
    return item_.getMaxStackSize(ItemDescriptor(item_, 0));
}

int EndstoneItemType::getMaxDurability() const
{
    return item_.getMaxDamage();
}

}  // namespace endstone::core
