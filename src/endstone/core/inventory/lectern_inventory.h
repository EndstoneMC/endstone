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

#include <functional>
#include <optional>
#include <utility>

#include "bedrock/world/level/block/actor/lectern_block_actor.h"
#include "endstone/core/inventory/inventory.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {

class EndstoneLecternInventory : public EndstoneInventoryBase<Inventory> {
public:
    using Provider = std::function<::LecternBlockActor &()>;

    explicit EndstoneLecternInventory(Provider provider) : provider_(std::move(provider)) {}

    void setItem(int index, std::optional<ItemStack> item) override
    {
        provider_().setItemFromBlock(
            index, item.has_value() ? EndstoneItemStack::toMinecraft(item.value()) : ::ItemStack::EMPTY_ITEM);
    }

private:
    [[nodiscard]] ::Container &getContainer() const override { return provider_(); }

    Provider provider_;
};

}  // namespace endstone::core
