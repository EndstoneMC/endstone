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

#include <bedrock/world/item/item_instance.h>
#include <bedrock/world/item/registry/item_registry_manager.h>
#include <pybind11/pybind11.h>

ItemStackBase::ItemStackBase()
{
    ItemStackBase::setNull(std::nullopt);
}

ItemStackBase::ItemStackBase(const BlockLegacy &block, int count)
{
    item_ = nullptr;
    user_data_ = nullptr;
    block_ = &block.getRenderBlock();
    init(block, count);
    _checkForItemWorldCompatibility();
}

ItemStackBase::ItemStackBase(const std::string_view name, int count, int aux_value, CompoundTag const *user_data)
{
    int aux = aux_value;
    if (const auto item = ItemRegistryManager::getItemRegistry().lookupByName(aux, name); !item.isNull()) {
        init(*item, count, aux, user_data, false);
    }
    else {
        ItemStackBase::setNull("Couldn't find item in registry.");
    }
    _checkForItemWorldCompatibility();
}

ItemStackBase::ItemStackBase(Item const &item, int count, int aux_value, CompoundTag const *user_data)
{
    init(item, count, aux_value, user_data, true);
    _checkForItemWorldCompatibility();
}

ItemStackBase::ItemStackBase(const ItemStackBase &rhs)
{
    block_ = rhs.block_;
    aux_value_ = rhs.aux_value_;
    if (block_ && aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
        init(block_->getLegacyBlock(), rhs.count_);
    }
    else {
        int id = 0;
        if (rhs.valid_deprecated_ && !rhs.item_.isNull()) {
            id = rhs.item_->getId();
        }
        else {
            id = Item::INVALID_ITEM_ID;
        }
        init(id, count_, aux_value_, false);
    }

    if (rhs.user_data_) {
        setUserData(std::move(rhs.user_data_->clone()));
    }

    show_pick_up_ = rhs.show_pick_up_;
    was_picked_up_ = rhs.was_picked_up_;
    if (this != &rhs) {
        can_place_on_ = rhs.can_place_on_;
        can_destroy_ = rhs.can_destroy_;
    }
    blocking_tick_ = rhs.blocking_tick_;
    _updateCompareHashes();
    _checkForItemWorldCompatibility();
}

ItemStackBase &ItemStackBase::operator=(const ItemStackBase &rhs)
{
    if (this != &rhs) {
        count_ = rhs.count_;
        aux_value_ = rhs.aux_value_;
        item_ = rhs.item_;
        block_ = rhs.block_;
        valid_deprecated_ = rhs.valid_deprecated_;
        pick_up_time_ = rhs.pick_up_time_;
        show_pick_up_ = rhs.show_pick_up_;
        was_picked_up_ = rhs.was_picked_up_;
        if (rhs.user_data_) {
            setUserData(rhs.user_data_->clone());
        }
        can_place_on_ = rhs.can_place_on_;
        can_destroy_ = rhs.can_destroy_;
        blocking_tick_ = rhs.blocking_tick_;
        _updateCompareHashes();
    }
    return *this;
}

ItemStackBase::~ItemStackBase() = default;

void ItemStackBase::reinit(Item const &, int, int) {}

void ItemStackBase::reinit(BlockLegacy const &, int) {}

void ItemStackBase::reinit(std::string_view, int, int) {}

ItemDescriptor ItemStackBase::getDescriptor() const
{
    if (block_) {
        if (aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
            return ItemDescriptor(block_->getLegacyBlock());
        }
        auto block_descriptor = ItemDescriptor(*block_);
        if (block_descriptor.getId() == getId()) {
            return std::move(block_descriptor);
        }
    }
    else if (!item_.isNull()) {
        return item_->buildDescriptor(aux_value_, user_data_.get());
    }
    return ItemDescriptor();
}

void ItemStackBase::setNull(std::optional<std::string> reason)
{
    if (user_data_) {
        user_data_->remove(TAG_CHARGED_ITEM);
    }
    charged_item_.reset();
    count_ = 0;
    block_ = nullptr;
    aux_value_ = 0;
    item_.reset();
    user_data_.reset();
    show_pick_up_ = true;
    was_picked_up_ = false;
    can_place_on_.clear();
    can_destroy_.clear();
    blocking_tick_ = Tick(0);
    _updateCompareHashes();
}

std::string ItemStackBase::toString() const
{
    std::stringstream ss;
    ss << count_;
    ss << " x ";
    if (hasCustomHoverName()) {
        ss << getCustomName();
    }
    else if (!item_.isNull()) {
        ss << item_->buildDescriptionName(*this);
    }
    ss << "(" << getId() << ")";
    ss << "@" << aux_value_;
    return ss.str();
}

std::string ItemStackBase::toDebugString() const
{
    std::stringstream ss;
    ss << "mItem = ";
    if (hasCustomHoverName()) {
        ss << getCustomName();
    }
    else if (!item_.isNull()) {
        ss << item_->buildDescriptionName(*this);
    }
    ss << ", id = " << getId();
    ss << ", mCount = " << std::to_string(count_);
    ss << ", mAuxValue = " << std::to_string(aux_value_);
    ss << ", mBlock = ";
    if (block_) {
        ss << block_->toDebugString();
    }
    else {
        ss << "null";
    }
    ss << ", mCanDestroyHash = " << std::to_string(can_destroy_hash_);
    ss << ", mCanPlaceOnHash = " << std::to_string(can_place_on_hash_);
    ss << ", mBlockTicking = " << std::to_string(blocking_tick_.tick_id);
    ss << ", mUserData = ";
    if (user_data_) {
        ss << user_data_->toString();
    }
    else {
        ss << "null";
    }
    ss << ", chargedItem = ";
    if (charged_item_ && !charged_item_->isNull()) {
        ss << charged_item_->item_->getFullItemName();
    }
    else {
        ss << "none";
    }
    return ss.str();
}

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

std::int16_t ItemStackBase::getId() const
{
    if (!valid_deprecated_) {
        return Item::INVALID_ITEM_ID;
    }
    if (item_.isNull()) {
        return 0;
    }
    return item_->getId();
}

std::uint16_t ItemStackBase::getAuxValue() const
{
    if (!block_ || aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
        return aux_value_;
    }
    return block_->data_;
}

std::string ItemStackBase::getName() const
{
    if (hasCustomHoverName()) {
        return getCustomName();
    }
    if (item_.isNull()) {
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

void ItemStackBase::setUserData(std::unique_ptr<CompoundTag> tag)
{
    user_data_ = std::move(tag);
    if (user_data_) {
        if (const auto *charged_item = user_data_->getCompound(TAG_CHARGED_ITEM)) {
            charged_item_ = std::make_unique<ItemInstance>(ItemInstance::fromTag(*charged_item));
        }
    }
}

const CompoundTag *ItemStackBase::getUserData() const
{
    return user_data_.get();
}

bool ItemStackBase::isBlock() const
{
    return !item_.isNull() && !item_->getLegacyBlock().isNull();
}

const Block *ItemStackBase::getBlock() const
{
    return block_;
}

void ItemStackBase::set(const std::uint8_t count)
{
    auto max_stack_size = -1;
    if (!item_.isNull()) {
        max_stack_size = item_->getMaxStackSize(getDescriptor());
    }
    if (count > max_stack_size) {
        count_ = max_stack_size;
    }
    else if (count == 0) {
        count_ = 0;
    }
    else {
        count_ = count;
    }
    if (!isNull()) {
        setNull(std::nullopt);
    }
}

std::uint8_t ItemStackBase::getCount() const
{
    return count_;
}

void ItemStackBase::_updateCompareHashes()
{
    static std::hash<const BlockLegacy *> hasher;
    std::ranges::sort(can_place_on_);
    can_place_on_hash_ = 0;
    for (const auto &block : can_place_on_) {
        can_place_on_hash_ ^= hasher(block) + 0x9e3779b9 + (can_place_on_hash_ << 6) + (can_place_on_hash_ >> 2);
    }

    std::ranges::sort(can_destroy_);
    can_destroy_hash_ = 0;
    for (const auto &block : can_destroy_) {
        can_destroy_hash_ ^= hasher(block) + 0x9e3779b9 + (can_destroy_hash_ << 6) + (can_destroy_hash_ >> 2);
    }
}

const std::string ItemStackBase::TAG_DISPLAY = "display";
const std::string ItemStackBase::TAG_DISPLAY_NAME = "Name";
const std::string ItemStackBase::TAG_CHARGED_ITEM = "chargedItem";
