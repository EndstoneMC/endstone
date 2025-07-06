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

EndstoneItemStack::EndstoneItemStack(const ::ItemStack &item)
    : ItemStack(getType(&item), item.getCount()), handle_(item.isNull() ? nullptr : const_cast<::ItemStack *>(&item))
{
}

EndstoneItemStack::EndstoneItemStack(const EndstoneItemStack &item) : ItemStack(item)
{
    handle_ = item.handle_;
    EndstoneItemStack::setItemMeta(item.getItemMeta().get());
}

bool EndstoneItemStack::isEndstoneItemStack() const
{
    return true;
}

const ItemType &EndstoneItemStack::getType() const
{
    return getType(handle_);
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
    if (type == "minecraft:air") {
        reset();
        return;
    }
    owned_handle_ = std::make_unique<::ItemStack>(type.getId(), 1);
    handle_ = owned_handle_.get();
}

int EndstoneItemStack::getAmount() const
{
    return handle_ != nullptr ? handle_->getCount() : 0;
}

void EndstoneItemStack::setAmount(int amount)
{
    const std::uint8_t count = amount & 0xff;
    if (count == 0) {
        reset();
    }
    if (handle_ == nullptr || handle_->isNull()) {
        return;
    }
    handle_->set(count);
}

int EndstoneItemStack::getData() const
{
    return handle_ != nullptr ? handle_->getAuxValue() : 0;
}

void EndstoneItemStack::setData(const int data)
{
    const auto aux_value = static_cast<std::int16_t>(data & 0X7fff);
    if (handle_ == nullptr || handle_->isNull()) {
        return;
    }
    handle_->setAuxValue(aux_value);
}

int EndstoneItemStack::getMaxStackSize() const
{
    if (handle_ == nullptr) {
        return 0;
    }
    return handle_->getMaxStackSize();
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
    if (handle_ == that.handle_) {
        return true;
    }
    if (handle_ == nullptr || that.handle_ == nullptr) {
        return false;
    }
    if (getType() != that.getType()) {
        return false;
    }
    return hasItemMeta() ? that.hasItemMeta() && handle_->getUserData()->equals(*that.handle_->getUserData())
                         : !that.hasItemMeta();
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta() const
{
    return getItemMeta(handle_);
}

bool EndstoneItemStack::hasItemMeta() const
{
    return hasItemMeta(handle_) && !EndstoneItemFactory::instance().equals(getItemMeta().get(), nullptr);
}

bool EndstoneItemStack::setItemMeta(ItemMeta *meta)
{
    return setItemMeta(handle_, meta);
}

std::unique_ptr<ItemStack> EndstoneItemStack::clone() const
{
    return std::make_unique<EndstoneItemStack>(*this);
}

::ItemStack EndstoneItemStack::toMinecraft(const ItemStack *item)
{
    if (item == nullptr || item->getType() == "minecraft:air") {
        return {};  // Empty item stack
    }
    if (item->isEndstoneItemStack()) {
        if (const auto *stack = static_cast<const EndstoneItemStack *>(item); stack->handle_) {
            return *stack->handle_;  // Call the copy constructor to make a copy
        }
        return {};  // Empty item stack
    }
    auto stack = ::ItemStack(item->getType().getId(), item->getAmount());
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

const ItemType &EndstoneItemStack::getType(const ::ItemStack *item)
{
    return (item && !item->isNull()) ? *ItemType::get(item->getItem()->getFullItemName())
                                     : *ItemType::get("minecraft:air");
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta(const ::ItemStack *item)
{
    const auto &type = getType(item);
    if (!hasItemMeta(item)) {
        return EndstoneItemFactory::instance().getItemMeta(getType(item));
    }

    // TODO(item): support map meta
    // if (type == "minecraft:filled_map") {
    //     return std::make_unique<EndstoneMapMeta>(item->getUserData());
    // }
    return std::make_unique<EndstoneItemMeta>(*item->getUserData());
}

bool EndstoneItemStack::hasItemMeta(const ::ItemStack *item)
{
    return item != nullptr && !item->isNull() && item->getUserData() != nullptr && !item->getUserData()->isEmpty();
}

bool EndstoneItemStack::setItemMeta(::ItemStack *item, const ItemMeta *meta)
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

void EndstoneItemStack::reset()
{
    handle_ = nullptr;
    owned_handle_.reset();
}

}  // namespace endstone::core
