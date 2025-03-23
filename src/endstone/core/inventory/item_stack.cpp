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

std::shared_ptr<ItemMeta> EndstoneItemStack::getItemMeta() const
{
    return getItemMeta(handle_);
}
bool EndstoneItemStack::hasItemMeta() const
{
    return hasItemMeta(handle_) && getItemMeta() != nullptr;
}

bool EndstoneItemStack::setItemMeta(std::shared_ptr<ItemMeta> meta)
{
    return setItemMeta(handle_, meta_);
}

::ItemStack EndstoneItemStack::toMinecraft(const std::shared_ptr<ItemStack> &item)
{
    if (!item || item->getType() == "minecraft:air") {
        return {};  // Empty item stack
    }
    if (item->isEndstoneItemStack()) {
        const auto stack = std::static_pointer_cast<EndstoneItemStack>(item);
        if (stack->handle_) {
            return *stack->handle_;  // Call the copy constructor to make a copy
        }
        return {};  // Empty item stack
    }
    auto stack = ::ItemStack(item->getType(), item->getAmount());
    if (item->hasItemMeta()) {
        setItemMeta(&stack, item->getItemMeta());
    }
    return stack;
}

std::shared_ptr<EndstoneItemStack> EndstoneItemStack::fromMinecraft(const ::ItemStack &item)
{
    if (item.isNull()) {
        return nullptr;
    }
    return std::make_shared<EndstoneItemStack>(item);
}

std::string EndstoneItemStack::getType(::ItemStack *item)
{
    return (item && !item->isNull()) ? item->getItem()->getFullItemName() : "minecraft:air";
}

std::shared_ptr<ItemMeta> EndstoneItemStack::getItemMeta(::ItemStack *item)
{
    if (item && !item->isNull()) {
        return EndstoneItemFactory::getItemMeta(getType(item), *item);
    }
    return ItemFactory::getItemMeta("minecraft:air");
}

bool EndstoneItemStack::hasItemMeta(::ItemStack *item)
{
    return !getItemMeta(item)->isEmpty();
}

bool EndstoneItemStack::setItemMeta(::ItemStack *item, const std::shared_ptr<ItemMeta> &meta)
{
    if (!item) {
        return false;
    }
    if (!meta) {
        EndstoneItemFactory::applyToItem(*item, ItemMeta::EMPTY.shared_from_this());
        return true;
    }
    // TODO(item): applicability check
    auto item_meta = ItemFactory::asMetaFor(getType(item), meta);
    if (!item_meta) {
        return true;
    }
    if (!item_meta->isEmpty()) {
        EndstoneItemFactory::applyToItem(*item, item_meta);
    }
    return true;
}

void EndstoneItemStack::reset()
{
    handle_ = nullptr;
    owned_handle_.reset();
}

}  // namespace endstone::core
