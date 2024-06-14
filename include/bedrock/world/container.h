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

#pragma once

#include <functional>

#include "bedrock/core/math/vec3.h"
#include "bedrock/forward.h"
#include "bedrock/world/item/item_stack.h"

class Container {
public:
    virtual ~Container() = 0;
    virtual void init() = 0;
    virtual void serverInitItemStackIds(int, int, std::function<void(int, ItemStack const &)>) = 0;
    virtual void addContentChangeListener(ContainerContentChangeListener *) = 0;
    virtual void removeContentChangeListener(ContainerContentChangeListener *) = 0;
    virtual void addRemovedListener(ContainerRemovedListener *) = 0;
    virtual void removeRemovedListener(ContainerRemovedListener *) = 0;
    [[nodiscard]] virtual ItemStack const &getItem(int) const = 0;
    virtual bool hasRoomForItem(ItemStack const &) = 0;
    virtual bool addItem(ItemStack &) = 0;
    virtual bool addItemWithForceBalance(ItemStack &) = 0;
    virtual bool addItemToFirstEmptySlot(ItemStack const &) = 0;
    virtual void setItem(int, ItemStack const &) = 0;
    virtual void setItemWithForceBalance(int, ItemStack const &, bool) = 0;
    virtual void removeItem(int, int) = 0;
    virtual void removeAllItems() = 0;
    virtual void removeAllItemsWithForceBalance() = 0;
    virtual void containerRemoved() = 0;
    virtual void dropSlotContent(BlockSource &, Vec3 const &, bool, int) = 0;
    virtual void dropContents(BlockSource &, Vec3 const &, bool) = 0;
    [[nodiscard]] virtual int getContainerSize() const = 0;
    [[nodiscard]] virtual int getMaxStackSize() const = 0;
    // ...
};
