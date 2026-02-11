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

#include "endstone/inventory/item_factory.h"
#include "endstone/inventory/item_type.h"

namespace endstone::core {

class EndstoneItemFactory : public ItemFactory {
public:
    static EndstoneItemFactory &instance();

    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta(ItemTypeId type) const override;
    [[nodiscard]] bool isApplicable(const ItemMeta *meta, ItemTypeId type) const override;
    [[nodiscard]] bool equals(const ItemMeta *meta1, const ItemMeta *meta2) const override;
    [[nodiscard]] std::unique_ptr<ItemMeta> asMetaFor(const ItemMeta *meta, ItemTypeId type) const override;

private:
    [[nodiscard]] std::unique_ptr<ItemMeta> getItemMeta(const ItemTypeId &type, const ItemMeta *meta) const;
};

}  // namespace endstone::core
