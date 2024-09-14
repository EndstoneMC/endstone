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

#include "bedrock/world/item/item_stack_base.h"

bool ItemStackBase::isNull() const
{
    if (valid_deprecated_) {
        const auto *item = getItem();
        if (!item || item->getFullItemName() != "minecraft:air") {
            if (count_ > 0 || block_ != nullptr || aux_value_ > 0) {
                return false;
            }
            if (item || user_data_ || can_destroy_hash_ > 0 || can_place_on_hash_) {
                return false;
            }
        }
    }
    return true;
}

bool ItemStackBase::hasCustomHoverName() const
{
    if (!user_data_) {
        return false;
    }
    const auto *tag = user_data_->getCompound(TAG_DISPLAY);
    if (!tag) {
        return false;
    }
    return tag->contains(TAG_DISPLAY_NAME);
}

std::string ItemStackBase::getCustomName() const
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

std::uint16_t ItemStackBase::getAuxValue() const
{
    if (!block_ || aux_value_ == 0x7fff) {
        return aux_value_;
    }
    return block_->data_;
}

std::string ItemStackBase::getName() const
{
    if (hasCustomHoverName()) {
        return getCustomName();
    }
    if (!item_) {
        return "";
    }
    return item_->buildDescriptionName(*this);
}

const Item *ItemStackBase::getItem() const
{
    return item_.get();
}

bool ItemStackBase::hasUserData() const
{
    return user_data_ != nullptr;
}

const CompoundTag *ItemStackBase::getUserData() const
{
    return user_data_.get();
}

bool ItemStackBase::isBlock() const
{
    return item_ && item_->getLegacyBlock();
}

const Block *ItemStackBase::getBlock() const
{
    return block_;
}

void ItemStackBase::set(std::uint8_t count)
{
    // TODO(fixme): implement this
    throw std::logic_error("Not implemented");
}

std::uint8_t ItemStackBase::getCount() const
{
    return count_;
}

const std::string ItemStackBase::TAG_DISPLAY = "display";
const std::string ItemStackBase::TAG_DISPLAY_NAME = "Name";
