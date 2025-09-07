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
#include "endstone/core/inventory/meta/item_meta.h"

namespace endstone::core {

EndstoneItemStack::EndstoneItemStack(const ::ItemStack &item) : ItemStack(getType(&item), item.getCount()), item_(item)
{
}

EndstoneItemStack::EndstoneItemStack(const EndstoneItemStack &item) : ItemStack(item)
{
    item_ = item.item_;
    EndstoneItemStack::setItemMeta(item.getItemMeta().get());
}

bool EndstoneItemStack::isEndstoneItemStack() const
{
    return true;
}

const ItemType &EndstoneItemStack::getType() const
{
    return getType(&item_);
}

Result<void> EndstoneItemStack::setType(const std::string &type)
{
    return ItemStack::setType(type);
}

void EndstoneItemStack::setType(const ItemType &type)
{
    if (getType() == type) {
        return;
    }
    item_.reinit(type.getId(), getAmount(), getData());
}

int EndstoneItemStack::getAmount() const
{
    return !item_.isNull() ? item_.getCount() : 0;
}

void EndstoneItemStack::setAmount(int amount)
{
    item_.set(amount);
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

::ItemStack EndstoneItemStack::toMinecraft(const ItemStack *item)
{
    if (item == nullptr || item->getType() == "minecraft:air") {
        return ::ItemStack::EMPTY_ITEM;
    }
    if (item->isEndstoneItemStack()) {
        auto *stack = static_cast<const EndstoneItemStack *>(item);
        if (stack->item_.isNull()) {
            return ::ItemStack::EMPTY_ITEM;
        }
        return ::ItemStack(stack->item_);  // Call the copy constructor to make a copy
    }
    auto stack = ::ItemStack(item->getType().getId(), item->getAmount(), item->getData());
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
    return (item && !item->isNull()) ? *ItemType::get(item->getItem()->getFullItemName())
                                     : *ItemType::get("minecraft:air");
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta(const ItemStackBase *item)
{
    if (!hasItemMeta(item)) {
        return EndstoneItemFactory::instance().getItemMeta(getType(item));
    }

    // TODO(item): support map meta
    // if (type == "minecraft:filled_map") {
    //     return std::make_unique<EndstoneMapMeta>(item->getUserData());
    // }
    return std::make_unique<EndstoneItemMeta>(*item->getUserData());
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

    if (!EndstoneItemFactory::instance().isApplicable(meta, getType(item))) {
        return false;
    }

    const auto item_meta = EndstoneItemFactory::instance().asMetaFor(meta, getType(item));
    if (!item_meta) {
        return true;
    }

    auto tag = std::make_unique<CompoundTag>();
    static_cast<const EndstoneItemMeta *>(item_meta.get())->applyToItem(*tag);
    item->setUserData(std::move(tag));
    return true;
}

}  // namespace endstone::core
