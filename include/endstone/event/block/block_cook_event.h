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

#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
/**
 * @brief Called when an ItemStack is successfully cooked in a block.
 */
class BlockCookEvent : public Cancellable<BlockEvent> {
public:
    ENDSTONE_EVENT(BlockCookEvent);

    BlockCookEvent(std::unique_ptr<Block> block, ItemStack source, ItemStack result)
        : Cancellable(std::move(block)), source_(std::move(source)), result_(std::move(result))
    {
    }

    /**
     * @brief Gets the smelted ItemStack for this event
     *
     * @return smelting source ItemStack
     */
    [[nodiscard]] const ItemStack &getSource() const { return source_; }

    /**
     * @brief Gets the resultant ItemStack for this event
     *
     * @return smelting result ItemStack
     */
    [[nodiscard]] const ItemStack &getResult() const { return result_; }

    /**
     * @brief Sets the resultant ItemStack for this event
     *
     * @param result new result ItemStack
     */
    void setResult(ItemStack result) { result_ = std::move(result); }

private:
    ItemStack source_;
    ItemStack result_;
};
}  // namespace endstone
