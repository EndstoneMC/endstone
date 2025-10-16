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

#include "endstone/core/inventory/item_factory.h"

#include "endstone/core/inventory/item_metas.h"
#include "endstone/core/inventory/item_type.h"

namespace endstone::core {
EndstoneItemFactory &EndstoneItemFactory::instance()
{
    static EndstoneItemFactory factory;
    return factory;
}

std::unique_ptr<ItemMeta> EndstoneItemFactory::getItemMeta(const std::string &type) const
{
    return getItemMeta(type, nullptr);
}

bool EndstoneItemFactory::isApplicable(const ItemMeta *meta, const std::string &type) const
{
    if (meta == nullptr) {
        return false;
    }
    const auto &details = EndstoneItemMetas::getItemMetaDetails(meta->getType());
    return details.applicableTo(type);
}

bool EndstoneItemFactory::equals(const ItemMeta *meta1, const ItemMeta *meta2) const
{
    if (meta1 == meta2 || (meta1 == nullptr && meta2 == nullptr)) {
        return true;
    }

    if (meta1 == nullptr) {
        return meta2->isEmpty();
    }

    if (meta2 == nullptr) {
        return meta1->isEmpty();
    }

    const auto &details1 = EndstoneItemMetas::getItemMetaDetails(meta1->getType());
    const auto &details2 = EndstoneItemMetas::getItemMetaDetails(meta2->getType());
    return details1.equalsCommon(*meta1, *meta2) && details1.notUncommon(*meta1, *meta2) &&
           details2.notUncommon(*meta2, *meta1);
}

std::unique_ptr<ItemMeta> EndstoneItemFactory::asMetaFor(const ItemMeta *meta, const std::string &type) const
{
    return getItemMeta(type, meta);
}

std::unique_ptr<ItemMeta> EndstoneItemFactory::getItemMeta(const std::string &type, const ItemMeta *meta) const
{
    const auto &details = EndstoneItemMetas::getItemMetaDetails(type);
    return details.fromItemMeta(type, meta);
}
}  // namespace endstone::core
