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

#include "endstone/core/inventory/meta/item_meta.h"

namespace endstone::core {

ItemMeta::Type EndstoneItemMeta::getType() const
{
    return Type::Base;
}

bool EndstoneItemMeta::hasLore() const
{
    return lore_.has_value() && !lore_.value().empty();
}

std::optional<std::vector<std::string>> EndstoneItemMeta::getLore() const
{
    if (!hasLore()) {
        return std::nullopt;
    }
    return lore_;
}

void EndstoneItemMeta::setLore(std::optional<std::vector<std::string>> lore)
{
    lore_ = std::move(lore);
    if (!hasLore()) {
        lore_ = std::nullopt;
    }
}

}  // namespace endstone::core
