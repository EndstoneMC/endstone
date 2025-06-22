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

#include "bedrock/world/item/item.h"
#include "endstone/inventory/item_type.h"
#include "endstone/namespaced_key.h"

namespace endstone::core {

class EndstoneItemType : public ItemType {
public:
    EndstoneItemType(NamespacedKey key, const ::Item &item) : key_(std::move(key)), item_(item) {}
    [[nodiscard]] std::string_view getId() const override;
    [[nodiscard]] NamespacedKey getKey() const override;
    [[nodiscard]] std::string getTranslationKey() const override;
    [[nodiscard]] int getMaxStackSize() const override;
    [[nodiscard]] int getMaxDurability() const override;

private:
    NamespacedKey key_;
    const ::Item &item_;
};

}  // namespace endstone::core
