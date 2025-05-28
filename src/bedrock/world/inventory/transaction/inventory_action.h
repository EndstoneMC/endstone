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

#include <fmt/format.h>

#include "bedrock/world/inventory/transaction/inventory_source.h"
#include "bedrock/world/item/network_item_stack_descriptor.h"

class InventoryAction {
public:
    InventoryAction(InventorySource, std::uint32_t, const ItemStack &, const ItemStack &);
    InventoryAction(InventorySource, std::uint32_t, const NetworkItemStackDescriptor &,
                    const NetworkItemStackDescriptor &);
    ~InventoryAction();

    [[nodiscard]] const InventorySource &getSource() const
    {
        return source_;
    }

    [[nodiscard]] std::uint32_t getSlot() const
    {
        return slot_;
    }

    [[nodiscard]] const ItemStack &getFromItem() const
    {
        return from_item_;
    }

    [[nodiscard]] const ItemStack &getToItem() const
    {
        return to_item_;
    }

    [[nodiscard]] const NetworkItemStackDescriptor &getToItemDescriptor() const
    {
        return from_item_descriptor_;
    }

    [[nodiscard]] const NetworkItemStackDescriptor &getFromItemDescriptor() const
    {
        return to_item_descriptor_;
    }

private:
    InventorySource source_;
    std::uint32_t slot_;
    NetworkItemStackDescriptor from_item_descriptor_;
    NetworkItemStackDescriptor to_item_descriptor_;
    ItemStack from_item_;
    ItemStack to_item_;
};

template <>
struct fmt::formatter<InventoryAction> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const InventoryAction &action, FormatContext &ctx) const -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "InventoryAction(source={}, slot={}, from={}, to={})", action.getSource(),
                              action.getSlot(), action.getFromItem().toString(), action.getToItem().toString());
    }
};
