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

#include "bedrock/symbol.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "bedrock/world/level/block/bedrock_block_names.h"

ItemStackBase::ItemStackBase()
{
    ItemStackBase::setNull(std::nullopt);
}

ItemStackBase::ItemStackBase(const BlockType &block, int count)
{
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
    if (this == &rhs) {
        return;
    }

    block_ = rhs.block_;
    aux_value_ = rhs.aux_value_;
    if (block_ && aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
        init(block_->getBlockType(), rhs.count_);
    }
    else {
        int id;
        if (rhs.valid_deprecated_) {
            if (!rhs.item_.isNull()) {
                id = rhs.item_->getId();
            }
            else {
                id = 0;
            }
        }
        else {
            id = Item::INVALID_ITEM_ID;
        }
        init(id, rhs.count_, aux_value_, false);
    }

    if (rhs.user_data_) {
        setUserData(std::move(rhs.user_data_->clone()));
    }

    show_pick_up_ = rhs.show_pick_up_;
    was_picked_up_ = rhs.was_picked_up_;
    can_place_on_ = rhs.can_place_on_;
    can_destroy_ = rhs.can_destroy_;
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

void ItemStackBase::reinit(Item const &item, int count, int aux_value) {}

void ItemStackBase::reinit(BlockType const &block, int count) {}

void ItemStackBase::reinit(std::string_view name, int count, int aux_value) {}

ItemDescriptor ItemStackBase::getDescriptor() const
{
    if (block_) {
        if (aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
            return ItemDescriptor(block_->getBlockType());
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
        if (!item || item->getFullNameHash() != BedrockBlockNames::Air) {
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

bool ItemStackBase::operator==(const ItemStackBase &other) const
{
    return matchesItem(other) && (count_ == other.count_);
}

bool ItemStackBase::operator!=(const ItemStackBase &other) const
{
    return !(*this == other);
}

bool ItemStackBase::matchesItem(const ItemStackBase &other) const  // NOLINT(*-no-recursion)
{
    if (item_.isNull()) {
        if (!other.item_.isNull()) {
            return false;
        }
    }
    else {
        if (other.item_.isNull() || *item_.get() != *other.item_.get()) {
            return false;
        }
    }

    if (aux_value_ != other.aux_value_) {
        return false;
    }

    if (block_ && block_ != other.block_) {
        return false;
    }

    if (!hasSameUserData(other)) {
        return false;
    }

    if (can_destroy_hash_ != other.can_destroy_hash_ || can_place_on_hash_ != other.can_place_on_hash_) {
        return false;
    }

    if (blocking_tick_.tick_id != other.blocking_tick_.tick_id) {
        return false;
    }

    bool has_charged_item = charged_item_ && !charged_item_->isNull();
    bool other_has_charged_item = other.charged_item_ && !other.charged_item_->isNull();
    if (has_charged_item != other_has_charged_item) {
        return false;
    }

    if (has_charged_item) {
        const ItemInstance &my_charge = *charged_item_;
        const ItemInstance &other_charge = other_has_charged_item ? *other.charged_item_ : ItemInstance::EMPTY_ITEM;

        if (!my_charge.matchesItem(other_charge)) {
            return false;
        }
        if (my_charge.count_ != other_charge.count_) {
            return false;
        }
    }

    return true;
}

void ItemStackBase::set(const int count)
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
    if (!*this) {
        setNull(std::nullopt);
    }
}

bool ItemStackBase::hasTag(const ItemTag &tag) const
{
    if (!item_.isNull()) {
        return item_->hasTag(tag);
    }
    return false;
}

bool ItemStackBase::hasUserData() const
{
    return user_data_ != nullptr;
}

bool ItemStackBase::hasSameUserData(const ItemStackBase &other) const
{
    if (isNull() && other.isNull()) {
        return true;
    }
    if (isNull() || other.isNull()) {
        return false;
    }

    const auto *my_tag = getUserData();
    const auto *other_tag = other.getUserData();
    const auto has_tags = my_tag != nullptr && !my_tag->isEmpty();
    const auto other_has_tags = other_tag != nullptr && !other_tag->isEmpty();
    if (!has_tags && !other_has_tags) {
        return true;
    }
    if (!has_tags || !other_has_tags) {
        return false;
    }
    return my_tag->equals(*other_tag);
}

void ItemStackBase::setUserData(std::unique_ptr<CompoundTag> tag)
{
    BEDROCK_CALL(&ItemStackBase::setUserData, this, std::move(tag));
}

const CompoundTag *ItemStackBase::getUserData() const
{
    return user_data_.get();
}

CompoundTag *ItemStackBase::getUserData()
{
    return user_data_.get();
}

const Item *ItemStackBase::getItem() const
{
    return item_.get();
}

WeakPtr<Item> ItemStackBase::getItemPtr() const
{
    return item_;
}

const Block *ItemStackBase::getBlock() const
{
    return block_;
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

bool ItemStackBase::isBlock() const
{
    return !item_.isNull() && !item_->getBlockType().isNull();
}

bool ItemStackBase::isValid_DeprecatedSeeComment() const
{
    return valid_deprecated_;
}

ItemStackBase::operator bool() const
{
    return isValid_DeprecatedSeeComment() && !item_.isNull() && !isNull() && count_ != 0;
}

std::uint8_t ItemStackBase::getMaxStackSize() const
{
    if (item_.isNull()) {
        return -1;
    }
    return item_->getMaxStackSize(getDescriptor());
}

std::int16_t ItemStackBase::getDamageValue() const
{
    if (item_.isNull()) {
        return 0;
    }
    return item_->getDamageValue(getUserData());
}

bool ItemStackBase::hasDamageValue() const
{
    if (item_.isNull()) {
        return false;
    }
    return item_->hasDamageValue(getUserData());
}

void ItemStackBase::removeDamageValue()
{
    if (!item_.isNull()) {
        item_->removeDamageValue(*this);
    }
}

void ItemStackBase::setDamageValue(std::int16_t damage)
{
    if (!item_.isNull()) {
        item_->setDamageValue(*this, damage);
    }
}

std::int16_t ItemStackBase::getAuxValue() const
{
    if (!block_ || aux_value_ == ItemDescriptor::ANY_AUX_VALUE) {
        return aux_value_;
    }
    return static_cast<std::int16_t>(block_->data_);
}

void ItemStackBase::setAuxValue(const std::int16_t aux_value)
{
    aux_value_ = aux_value;
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

std::uint8_t ItemStackBase::getCount() const
{
    return count_;
}

void ItemStackBase::init(const BlockType &block, const int count)
{
    init(block.getBlockItemId(), count, 0, true);
    if (!item_.isNull() && item_->getRequiredBaseGameVersion() == BaseGameVersion::EMPTY) {
        item_->setMinRequiredBaseGameVersion(block.getRequiredBaseGameVersion());
    }
}

void ItemStackBase::init(const Item &item, int count, int aux_value, const CompoundTag *user_data, bool do_remap)
{
    const auto &block_type = item.getBlockType();
    const auto id = item.getId();
    if (!block_type.isNull()) {
        if (id < ItemRegistry::START_ITEM_ID) {
            if (aux_value == ItemDescriptor::ANY_AUX_VALUE) {
                block_ = &block_type->getRenderBlock();
                init(*block_type, count);
                aux_value_ = ItemDescriptor::ANY_AUX_VALUE;
            }
            else {
                block_ = block_type->tryGetStateFromLegacyData(aux_value);
                if (!block_) {
                    init(*block_type, count);
                }
                else {
                    init(block_->getBlockType(), count);
                }
            }
        }
        else {
            block_ = block_type->tryGetStateFromLegacyData(aux_value);
            init(id, count, aux_value, true);
        }
    }
    else {
        init(id, count, aux_value, do_remap);
    }
    if (user_data) {
        setUserData(user_data->clone());
    }
}

void ItemStackBase::init(const int id, const int count, const int aux_value, const bool do_remap)
{
    void (ItemStackBase::*fp)(int id, int count, int aux_value, bool do_remap) = &ItemStackBase::init;
    BEDROCK_CALL(fp, this, id, count, aux_value, do_remap);
}

void ItemStackBase::_updateCompareHashes()
{
    static std::hash<const BlockType *> hasher;
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

void ItemStackBase::_checkForItemWorldCompatibility()
{
    const auto registry_ref = ItemRegistryManager::getItemRegistry();
    auto lock = registry_ref.lockItemWorldCompatibilityMutex();
    if (!registry_ref.shouldCheckForItemWorldCompatibility()) {
        return;
    }

    auto world_version = registry_ref.getWorldBaseGameVersion();
    bool compatible = false;
    if (!item_.isNull()) {
        compatible = item_->getRequiredBaseGameVersion().isCompatibleWith(world_version);
    }
    else if (block_) {
        compatible = block_->getBlockType().getRequiredBaseGameVersion().isCompatibleWith(world_version);
    }

    if (!compatible) {
        setNull("Item is not compatible with the base game version.");
    }
}

const std::string ItemStackBase::TAG_DISPLAY = "display";
const std::string ItemStackBase::TAG_DISPLAY_NAME = "Name";
const std::string ItemStackBase::TAG_LORE = "Lore";
const std::string ItemStackBase::TAG_CHARGED_ITEM = "chargedItem";
const std::string ItemStackBase::TAG_REPAIR_COST = "RepairCost";
const std::string ItemStackBase::TAG_ENCHANTS = "ench";
