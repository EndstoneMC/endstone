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
#include "item_type.h"

namespace endstone::core {

EndstoneItemStack::EndstoneItemStack(const ::ItemStack &item)
    : handle_(item.isNull() ? nullptr : const_cast<::ItemStack *>(&item))
{
}

bool EndstoneItemStack::isEndstoneItemStack() const
{
    return true;
}

std::string EndstoneItemStack::getType() const
{
    return getType(handle_);
}

void EndstoneItemStack::setType(std::string type)
{
    if (getType() == type) {
        return;
    }
    if (type == "minecraft:air") {
        reset();
        return;
    }
    owned_handle_ = std::make_unique<::ItemStack>(type, 1);
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

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta() const
{
    return getItemMeta(handle_);
}

bool EndstoneItemStack::hasItemMeta() const
{
    return hasItemMeta(handle_) && getItemMeta() != nullptr;
}

bool EndstoneItemStack::setItemMeta(ItemMeta *meta)
{
    return setItemMeta(handle_, meta);
}

::ItemStack EndstoneItemStack::toMinecraft(const ItemStack *item)
{
    if (!item || item->getType() == "minecraft:air") {
        return {};  // Empty item stack
    }
    if (item->isEndstoneItemStack()) {
        if (const auto *stack = static_cast<const EndstoneItemStack *>(item); stack->handle_) {
            return *stack->handle_;  // Call the copy constructor to make a copy
        }
        return {};  // Empty item stack
    }
    auto stack = ::ItemStack(item->getType(), item->getAmount());
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

std::string EndstoneItemStack::getType(const ::ItemStack *item)
{
    return (item && !item->isNull()) ? item->getItem()->getFullItemName() : "minecraft:air";
}

std::unique_ptr<ItemMeta> EndstoneItemStack::getItemMeta(const ::ItemStack *item)
{
    if (!hasItemMeta(item)) {
        return EndstoneItemFactory::instance().getItemMeta(getType(item));
    }
    const auto type = EndstoneItemType::fromMinecraft(*item->getItem());
    return static_cast<EndstoneItemType *>(type.get())->getItemMeta(*item);
}

bool EndstoneItemStack::hasItemMeta(const ::ItemStack *item)
{
    return item != nullptr && !item->isNull() && item->getUserData() != nullptr;
}

bool EndstoneItemStack::setItemMeta(::ItemStack *item, const ItemMeta *meta)
{
    if (!item) {
        return false;
    }
    if (!meta) {
        EndstoneItemMeta::restoreMeta(*item);
        return true;
    }
    // TODO(item): applicability check
    const auto item_meta = EndstoneItemFactory::instance().asMetaFor(meta, getType(item));
    if (!item_meta) {
        return true;
    }

    EndstoneItemMeta::applyToItem(*item_meta, *item);
    return true;
}

void EndstoneItemStack::reset()
{
    handle_ = nullptr;
    owned_handle_.reset();
}

}  // namespace endstone::core
