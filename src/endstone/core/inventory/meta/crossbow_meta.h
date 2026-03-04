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

#include "endstone/core/inventory/meta/item_meta.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/inventory/meta/crossbow_meta.h"

namespace endstone::core {
class EndstoneCrossbowMeta : public EndstoneItemMetaBase<CrossbowMeta> {
public:
    explicit EndstoneCrossbowMeta(const ItemMeta *meta);
    explicit EndstoneCrossbowMeta(const ::CompoundTag &tag);

    [[nodiscard]] bool hasChargedProjectile() const override;
    [[nodiscard]] std::optional<ItemStack> getChargedProjectile() const override;
    void setChargedProjectile(std::optional<ItemStack> projectile) override;

    void applyToItem(::CompoundTag &tag) const override;
    [[nodiscard]] bool isEmpty() const override;
    [[nodiscard]] bool equalsCommon(const ItemMeta &meta) const override;
    [[nodiscard]] bool notUncommon(const ItemMeta &meta) const override;
    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override;

private:
    [[nodiscard]] bool isCrossbowEmpty() const;

    std::optional<ItemStack> charged_projectile_;
};
}  // namespace endstone::core
