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

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

#include "bedrock/core/memory.h"
#include "bedrock/forward.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_legacy.h"
#include "bedrock/world/level/tick.h"

class ItemStackBase {
public:
    virtual ~ItemStackBase() = 0;
    virtual void reinit(Item const &, int, int) = 0;
    virtual void reinit(BlockLegacy const &, int) = 0;
    virtual void reinit(std::string_view, int, int) = 0;
    virtual void setNull(std::optional<std::string>) = 0;
    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual std::string toDebugString() const = 0;

    [[nodiscard]] bool hasCustomHoverName() const
    {
        if (!user_data_) {
            return false;
        }
        const auto *tag = user_data_->getCompound(TAG_DISPLAY);
        if (!tag) {
            return false;
        }
        return tag->contains(TAG_DISPLAY_NAME);
    };

    [[nodiscard]] std::string getCustomName() const
    {
        if (user_data_) {
            if (const auto *tag = user_data_->getCompound(TAG_DISPLAY); tag) {
                if (tag->contains(TAG_DISPLAY_NAME)) {
                    return tag->getString(TAG_DISPLAY_NAME);
                }
            }
        }
        return "";
    }

    [[nodiscard]] std::uint16_t getAuxValue() const
    {
        if (!block_ || aux_value_ == 0x7fff) {
            return aux_value_;
        }
        return block_->data_;
    }

    [[nodiscard]] std::string getName() const
    {
        if (hasCustomHoverName()) {
            return getCustomName();
        }
        if (!item_) {
            return "";
        }
        return item_->buildDescriptionName(*this);
    }

    [[nodiscard]] const Item *getItem() const
    {
        return item_.get();
    }

    [[nodiscard]] bool hasUserData() const
    {
        return user_data_ != nullptr;
    }

    [[nodiscard]] const CompoundTag *getUserData() const
    {
        return user_data_.get();
    }

    [[nodiscard]] bool isBlock() const
    {
        return item_ && item_->getLegacyBlock();
    }

    [[nodiscard]] const Block *getBlock() const
    {
        return block_;
    }

    [[nodiscard]] std::uint8_t getCount() const  // Endstone
    {
        return count_;
    }

private:
    inline const static std::string TAG_DISPLAY = "display";    // NOLINT(*-identifier-naming)
    inline const static std::string TAG_DISPLAY_NAME = "Name";  // NOLINT(*-identifier-naming)

    WeakPtr<Item> item_;                                  // +8
    std::unique_ptr<CompoundTag> user_data_;              // +16
    Block *block_;                                        // +24
    std::uint16_t aux_value_;                             // +32
    std::uint8_t count_;                                  // +34
    bool valid_;                                          // +35
    std::chrono::steady_clock::time_point pick_up_time_;  // +40
    bool show_pick_up_;                                   // +48
    bool was_picked_up_;                                  // +49
    std::vector<const BlockLegacy *> can_place_on_;       // +56
    std::uint64_t can_place_on_hash_;                     // +80
    std::vector<const BlockLegacy *> can_destroy_;        // +88
    std::uint64_t can_destroy_hash_;                      // +112
    Tick blocking_tick_;                                  // +120
    std::unique_ptr<ItemInstance> charged_item_;          // +128
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackBase, 136, 136);
