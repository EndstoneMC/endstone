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

#include "endstone/core/block/campfire.h"

#include "endstone/check.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {

EndstoneCampfire::EndstoneCampfire(const EndstoneBlock &block, ::CampfireBlockActor &campfire, bool use_snapshot)
    : EndstoneBlockStateBase<Campfire>(block, campfire, use_snapshot),
      campfire_(static_cast<::CampfireBlockActor &>(*getBlockActor()))
{
}

int EndstoneCampfire::getSize() const
{
    return ::CampfireBlockActor::MAX_COOKING_ITEMS;
}

std::optional<ItemStack> EndstoneCampfire::getItem(int index) const
{
    checkIndex(index);
    const auto &item = campfire_.getCookingItem(index);
    if (item.isNull()) {
        return std::nullopt;
    }
    return EndstoneItemStack::fromMinecraft(item);
}

void EndstoneCampfire::setItem(int index, const std::optional<ItemStack> &item)
{
    checkIndex(index);
    if (!item.has_value()) {
        campfire_.setCookingItem(index, ItemInstance::EMPTY_ITEM);
    }
    else {
        campfire_.setCookingItem(index, ItemInstance(EndstoneItemStack::toMinecraft(item.value())));
    }
    campfire_.setChanged();
}

int EndstoneCampfire::getCookTime(int index) const
{
    checkIndex(index);
    return campfire_.getCookingTime(index);
}

void EndstoneCampfire::setCookTime(int index, int cook_time)
{
    checkIndex(index);
    campfire_.setCookingTime(index, cook_time);
    campfire_.setChanged();
}

void EndstoneCampfire::checkIndex(int index) const
{
    Preconditions::checkArgument(index >= 0 && index < ::CampfireBlockActor::MAX_COOKING_ITEMS,
                                 "Campfire slot must be between 0 and {}, got {}.",
                                 ::CampfireBlockActor::MAX_COOKING_ITEMS - 1, index);
}

}  // namespace endstone::core
