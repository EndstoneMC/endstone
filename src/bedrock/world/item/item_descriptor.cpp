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

#include "bedrock/world/item/item_descriptor.h"

#include "bedrock/bedrock.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/registry/item_registry_manager.h"

struct InternalItemDescriptor : ItemDescriptor::BaseDescriptor {
    InternalItemDescriptor(WeakPtr<Item> &&item, std::int16_t aux_value);
    [[nodiscard]] std::unique_ptr<BaseDescriptor> clone() const override;
    [[nodiscard]] bool sameItem(const ItemDescriptor::ItemEntry &, bool) const override;
    [[nodiscard]] std::string getFullName() const override;
    [[nodiscard]] ItemDescriptor::ItemEntry getItem() const override;
    [[nodiscard]] std::map<std::string, std::string> toMap() const override;
    [[nodiscard]] std::optional<CompoundTag> save() const override;
    void serialize(BinaryStream &) const override;
    [[nodiscard]] ItemDescriptor::InternalType getType() const override;
    [[nodiscard]] size_t getHash() const override;

private:
    ItemDescriptor::ItemEntry item_entry_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(InternalItemDescriptor, 24, 24);

InternalItemDescriptor::InternalItemDescriptor(WeakPtr<Item> &&item, const std::int16_t aux_value)
    : item_entry_(item.get(), aux_value)
{
}

std::unique_ptr<ItemDescriptor::BaseDescriptor> InternalItemDescriptor::clone() const
{
    return std::make_unique<InternalItemDescriptor>(*this);
}

bool InternalItemDescriptor::sameItem(const ItemDescriptor::ItemEntry &other, bool compare_aux) const
{
    if (item_entry_.item != other.item) {
        return false;
    }

    if (!compare_aux || item_entry_.aux_value == ItemDescriptor::ANY_AUX_VALUE ||
        other.aux_value == ItemDescriptor::ANY_AUX_VALUE) {
        return true;
    }

    const auto *block = item_entry_.getBlock();
    if (const auto *other_block = other.getBlock(); block && other_block) {
        return block == other_block;
    }
    return item_entry_.aux_value == other.aux_value;
}

std::string InternalItemDescriptor::getFullName() const
{
    return item_entry_.item->getFullItemName();
}

ItemDescriptor::ItemEntry InternalItemDescriptor::getItem() const
{
    return item_entry_;
}

std::map<std::string, std::string> InternalItemDescriptor::toMap() const
{
    std::map<std::string, std::string> result;
    result["name"] = getFullName();
    return result;
}

std::optional<CompoundTag> InternalItemDescriptor::save() const
{
    if (!item_entry_.item) {
        return std::nullopt;
    }
    CompoundTag tag;
    tag.putString("Name", getFullName());
    tag.putShort("Aux", item_entry_.aux_value);
    return tag;
}

void InternalItemDescriptor::serialize(BinaryStream &stream) const
{
    stream.writeIf(
        item_entry_.item != nullptr, "item exist?",
        [&](BinaryStream &s) {  // true
            s.writeSignedShort(item_entry_.item->getId(), "Id", nullptr);
            s.writeSignedShort(item_entry_.aux_value, "Aux Value", nullptr);
        },
        [&](BinaryStream &s) {  // false
            s.writeSignedShort(0, "Zero value", nullptr);
        });
}

ItemDescriptor::InternalType InternalItemDescriptor::getType() const
{
    return ItemDescriptor::InternalType::Default;
}

size_t InternalItemDescriptor::getHash() const
{
    static std::hash<std::int64_t> hasher;

    const auto *item = item_entry_.item;
    if (!item) {
        return ItemDescriptor::EMPTY_INGREDIENT_HASH;
    }
    auto hash = item->getFullNameHash().getHash();
    return hash ^ (item_entry_.aux_value + hasher(item->getId() << 16) + 16 * hash + 0x9E3779B9LL);
}

ItemDescriptor::ItemDescriptor(const Block &block)
{
    const auto registry_ref = ItemRegistryManager::getItemRegistry();
    const auto block_item_id = block.getBlockType().getBlockItemId();
    if (auto item = registry_ref.getItem(block_item_id); !item.isNull()) {
        impl_ = std::make_unique<InternalItemDescriptor>(std::move(item), block.data_);
    }
}

ItemDescriptor::ItemDescriptor(const BlockType &block)
{
    const auto registry_ref = ItemRegistryManager::getItemRegistry();
    const auto block_item_id = block.getBlockItemId();
    if (auto item = registry_ref.getItem(block_item_id); !item.isNull()) {
        impl_ = std::make_unique<InternalItemDescriptor>(std::move(item), ANY_AUX_VALUE);
    }
}

ItemDescriptor::ItemDescriptor(const Item &item, int aux_value)
{
    const auto registry_ref = ItemRegistryManager::getItemRegistry();
    if (auto item_ptr = registry_ref.getItem(item.getId()); !item_ptr.isNull()) {
        impl_ = std::make_unique<InternalItemDescriptor>(std::move(item_ptr), aux_value > 0 ? aux_value : 0);
    }
    else {
        impl_ = std::make_unique<InternalItemDescriptor>(nullptr, aux_value > 0 ? aux_value : 0);
    }
}

ItemDescriptor::ItemDescriptor(const ItemDescriptor &other)
{
    if (other.impl_) {
        impl_ = std::move(other.impl_->clone());
    }
}

void ItemDescriptor::serialize(Json::Value &json) const
{
    if (impl_) {
        impl_->serialize(json);
    }
}

void ItemDescriptor::serialize(BinaryStream &stream) const
{
    if (impl_ && impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    auto type = impl_ ? impl_->getType() : InternalType::Invalid;
    stream.writeByte(static_cast<std::uint8_t>(type), "InternalType", nullptr);
    if (impl_) {
        impl_->serialize(stream);
    }
}

const Item *ItemDescriptor::getItem() const
{
    if (!impl_) {
        return nullptr;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_->getItem().item;
}

const Block *ItemDescriptor::getBlock() const
{
    if (!impl_) {
        return nullptr;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_->getItem().getBlock();
}

bool ItemDescriptor::isValid(bool should_resolve) const
{
    if (should_resolve && impl_ && impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_ != nullptr;
}

std::int16_t ItemDescriptor::getId() const
{
    if (!impl_) {
        return Item::INVALID_ITEM_ID;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    if (const auto *item = impl_->getItem().item; item) {
        return item->getId();
    }
    return 0;
}

std::int16_t ItemDescriptor::getAuxValue() const
{
    if (!impl_) {
        return ANY_AUX_VALUE;
    }
    if (impl_->shouldResolve()) {
        impl_ = std::move(impl_->resolve());
    }
    return impl_->getItem().aux_value;
}

std::string ItemDescriptor::getFullName() const
{
    static std::string empty;
    if (!impl_) {
        return empty;
    }
    return impl_->getFullName();
}

ItemDescriptor::ItemEntry::ItemEntry(const Item *item, std::int16_t aux_value) : item(item), aux_value(aux_value) {}

const Block *ItemDescriptor::ItemEntry::getBlock() const
{
    if (!item) {
        return nullptr;
    }
    const auto &block_type = item->getBlockType();
    if (block_type.isNull()) {
        return nullptr;
    }
    if (aux_value == ANY_AUX_VALUE) {
        return &block_type->getRenderBlock();
    }
    return block_type->tryGetStateFromLegacyData(aux_value);
}

bool ItemDescriptor::BaseDescriptor::sameItems(BaseDescriptor const &, bool flag) const
{
    if (const auto item = getItem(); item.item) {
        return sameItem(item, flag);
    }
    return false;
}

std::string ItemDescriptor::BaseDescriptor::getFullName() const
{
    return "";
}

std::string ItemDescriptor::BaseDescriptor::toString() const
{
    return getFullName();
}

bool ItemDescriptor::BaseDescriptor::forEachItemUntil(std::function<bool(Item const &, std::int16_t)> func) const
{
    if (const auto item = getItem(); item.item) {
        return func(*item.item, item.aux_value);
    }
    return false;
}

void ItemDescriptor::BaseDescriptor::serialize(Json::Value &val) const
{
    val = getFullName();
}

bool ItemDescriptor::BaseDescriptor::isValid() const
{
    return true;
}

bool ItemDescriptor::BaseDescriptor::shouldResolve() const
{
    return false;
}

std::unique_ptr<ItemDescriptor::BaseDescriptor> ItemDescriptor::BaseDescriptor::resolve() const
{
    return nullptr;
}
