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

#include "endstone/core/inventory/item_stack.h"

#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_factory.h"
#include "endstone/core/inventory/item_metas.h"
#include "endstone/core/inventory/meta/item_meta.h"

namespace endstone::core {

EndstoneItemStack::EndstoneItemStack(const ::ItemStackBase &item) : item_(item) {}

EndstoneItemStack::EndstoneItemStack(const EndstoneItemStack &rhs) : ItemStack(rhs), item_(rhs.item_)
{
    EndstoneItemStack::setItemMeta(rhs.getItemMeta().get());
}

bool EndstoneItemStack::isEndstoneItemStack() const
{
    return true;
}

const ItemType &EndstoneItemStack::getType() const
{
    return getType(&item_);
}

void EndstoneItemStack::setType(ItemTypeId type)
{
    const auto *item_type = ItemType::get(type);
    Preconditions::checkArgument(item_type != nullptr, "Unknown item type: {}", type);
    item_.reinit(std::string(type), getAmount(), getData());
}

int EndstoneItemStack::getAmount() const
{
    return !item_.isNull() ? item_.getCount() : 0;
}

void EndstoneItemStack::setAmount(int amount)
{
    Preconditions::checkArgument(amount >= 1 && amount <= 0xff, "Item stack amount must be between 1 to 255, got {}.",
                                 amount);
    item_.set(amount & 0xff);
}

int EndstoneItemStack::getData() const
{
    return !item_.isNull() ? item_.getAuxValue() : 0;
}

void EndstoneItemStack::setData(const int data)
{
    const auto aux_value = static_cast<std::int16_t>(data & 0x7fff);
    if (item_.isNull()) {
        return;
    }
    item_.setAuxValue(aux_value);
}

std::string EndstoneItemStack::getTranslationKey() const
{
    if (item_.isNull()) {
        return "item.air.name";
    }
    return item_.getDescriptionId();
}

int EndstoneItemStack::getMaxStackSize() const
{
    if (item_.isNull()) {
        return 0;
    }
    return item_.getMaxStackSize();
}

bool EndstoneItemStack::isSimilar(const ItemStack &other) const
{
    if (&other == this) {
        return true;
    }
    if (!other.isEndstoneItemStack()) {
        return other.isSimilar(*this);
    }

    const auto &that = static_cast<const EndstoneItemStack &>(other);
    if (&item_ == &that.item_) {
        return true;
    }
    if (getType() != that.getType()) {
        return false;
    }
    return hasItemMeta() ? that.hasItemMeta() && item_.getUserData()->equals(*that.item_.getUserData())
                         : !that.hasItemMeta();
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta() const
{
    return getItemMeta(&item_);
}

bool EndstoneItemStack::hasItemMeta() const
{
    return hasItemMeta(&item_) && !EndstoneItemFactory::instance().equals(getItemMeta().get(), nullptr);
}

bool EndstoneItemStack::setItemMeta(ItemMeta *meta)
{
    return setItemMeta(&item_, meta);
}

std::unique_ptr<ItemStack> EndstoneItemStack::clone() const
{
    return std::make_unique<EndstoneItemStack>(*this);
}

CompoundTag EndstoneItemStack::toNbt() const
{
    CompoundTag tag;
    const auto *item = item_.getItem();
    tag["Name"] = StringTag(item == nullptr ? "" : item->getSerializedName());
    tag["Count"] = ByteTag(item_.getCount());
    tag["Damage"] = ShortTag(item_.getAuxValue());
    tag["WasPickedUp"] = ByteTag(item_.getWasPickedUp());
    if (const auto *block = item_.getBlock()) {
        tag["Block"] = nbt::fromMinecraft(block->getSerializationId());
    }
    if (const auto *user_data = item_.getUserData()) {
        tag["tag"] = nbt::fromMinecraft(*user_data);
    }
    if (auto &can_place_on = item_.getCanPlaceOn(); !can_place_on.empty()) {
        ListTag list;
        for (const auto &block : can_place_on) {
            list.emplace_back(StringTag(block->getName().getString()));
        }
        tag[ItemStackBase::TAG_STORE_CAN_PLACE_ON] = list;
    }
    if (auto &can_destroy = item_.getCanDestroy(); !can_destroy.empty()) {
        ListTag list;
        for (const auto &block : can_destroy) {
            list.emplace_back(StringTag(block->getName().getString()));
        }
        tag[ItemStackBase::TAG_STORE_CAN_DESTROY] = list;
    }
    return tag;
}

::ItemStack EndstoneItemStack::toMinecraft(const ItemStack *item)
{
    if (item == nullptr || item->getType() == ItemType::Air) {
        return ::ItemStack::EMPTY_ITEM;
    }

    if (item->isEndstoneItemStack()) {
        auto *stack = static_cast<const EndstoneItemStack *>(item);
        if (stack->item_.isNull()) {
            return ::ItemStack::EMPTY_ITEM;
        }
        return ::ItemStack(stack->item_);  // Call the copy constructor to make a copy
    }

    auto stack = ::ItemStack(std::string(item->getType().getId()), item->getAmount(), item->getData());
    if (item->hasItemMeta()) {
        setItemMeta(&stack, item->getItemMeta().get());
    }
    return stack;
}

std::unique_ptr<EndstoneItemStack> EndstoneItemStack::fromMinecraft(const ::ItemStack &item)
{
    if (item.isNull()) {
        return nullptr;
    }
    return std::make_unique<EndstoneItemStack>(item);
}

const ItemType &EndstoneItemStack::getType(const ItemStackBase *item)
{
    return (item && *item) ? *ItemType::get(item->getItem()->getFullItemName()) : *ItemType::get(ItemType::Air);
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta(const ItemStackBase *item)
{
    const auto &type = getType(item);
    if (!hasItemMeta(item)) {
        return EndstoneItemFactory::instance().getItemMeta(getType(item));
    }
    return EndstoneItemMetas::getItemMetaDetails(type).fromItemStack(*item);
}

bool EndstoneItemStack::hasItemMeta(const ItemStackBase *item)
{
    return item != nullptr && !item->isNull() && item->getUserData() != nullptr && !item->getUserData()->isEmpty();
}

bool EndstoneItemStack::setItemMeta(ItemStackBase *item, const ItemMeta *meta)
{
    if (item == nullptr || item->isNull()) {
        return false;
    }

    if (EndstoneItemFactory::instance().equals(meta, nullptr)) {
        item->setUserData(nullptr);
        return true;
    }

    const auto &type = getType(item);
    if (!EndstoneItemFactory::instance().isApplicable(meta, type)) {
        return false;
    }

    const auto item_meta = EndstoneItemFactory::instance().asMetaFor(meta, type);
    if (!item_meta) {
        return true;
    }

    if (const auto &m = static_cast<EndstoneItemMeta &>(*item_meta); !m.isEmpty()) {
        auto tag = item->hasUserData() ? item->getUserData()->clone() : std::make_unique<::CompoundTag>();
        m.applyToItem(*tag);
        item->setUserData(std::move(tag));
    }

    return true;
}

}  // namespace endstone::core
