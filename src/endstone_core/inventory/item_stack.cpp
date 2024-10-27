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

#include "endstone/detail/inventory/item_stack.h"

namespace endstone::detail {

EndstoneItemStack::EndstoneItemStack(::ItemStack &item) : handle_(item.isNull() ? nullptr : &item) {}

std::string EndstoneItemStack::getType() const
{
    // TODO: add BedrockBlockNames::Air = HashedString("minecraft:air")
    return (handle_ && !handle_->isNull()) ? handle_->getItem()->getFullItemName() : "minecraft:air";
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
    owned_handle_ = ::ItemStack::create(type, 1);
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

std::shared_ptr<EndstoneItemStack> EndstoneItemStack::fromMinecraft(::ItemStack &item)
{
    if (item.isNull()) {
        return nullptr;
    }
    return std::make_shared<EndstoneItemStack>(item);
}

const EndstoneItemStack *EndstoneItemStack::asEndstoneItemStack() const
{
    return this;
}

EndstoneItemStack *EndstoneItemStack::asEndstoneItemStack()
{
    return this;
}

void EndstoneItemStack::reset()
{
    handle_ = nullptr;
    owned_handle_.reset();
}

}  // namespace endstone::detail
