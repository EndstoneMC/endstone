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

EndstoneItemStack::EndstoneItemStack(const EndstoneItemStack &other) : item_(other.item_) {}

EndstoneItemStack &EndstoneItemStack::operator=(const EndstoneItemStack &other)
{
    if (this != &other) {
        item_ = ItemInstance(other.item_);
    }
    return *this;
}

std::unique_ptr<ItemStack::Impl> EndstoneItemStack::clone() const
{
    return std::make_unique<EndstoneItemStack>(*this);
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

bool EndstoneItemStack::isSimilar(const Impl &other) const
{
    if (&other == this) {
        return true;
    }
    const auto &rhs = static_cast<const EndstoneItemStack &>(other);
    if (&item_ == &rhs.item_) {
        return true;
    }
    if (getType() != rhs.getType()) {
        return false;
    }
    return hasItemMeta() ? rhs.hasItemMeta() && item_.getUserData()->equals(*rhs.item_.getUserData())
                         : !rhs.hasItemMeta();
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta() const
{
    return getItemMeta(&item_);
}

bool EndstoneItemStack::hasItemMeta() const
{
    return hasItemMeta(&item_) && !EndstoneItemFactory::instance().equals(getItemMeta().get(), nullptr);
}

bool EndstoneItemStack::setItemMeta(const ItemMeta *meta)
{
    return setItemMeta(&item_, meta);
}

::ItemStack EndstoneItemStack::toMinecraft(const ItemStack &item)
{
    const auto &impl = static_cast<const EndstoneItemStack &>(*item.impl_);
    if (impl.item_.isNull()) {
        return ::ItemStack::EMPTY_ITEM;
    }
    return ::ItemStack(impl.item_);
}

ItemStack EndstoneItemStack::fromMinecraft(const ::ItemStackBase &item)
{
    return ItemStack(std::make_unique<EndstoneItemStack>(item));
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
