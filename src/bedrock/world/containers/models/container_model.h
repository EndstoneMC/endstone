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

#include <string>
#include <vector>

#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"

class ContainerWeakRef;
class ItemInstance;
class ItemStackBase;

enum class ContainerExpandStatus : int;

class ContainerModel : public ContainerContentChangeListener {
public:
    ~ContainerModel() override = 0;
    virtual void postInit() = 0;
    virtual void releaseResources() = 0;
    void containerContentChanged(int) override = 0;
    [[nodiscard]] virtual int getContainerSize() const = 0;
    [[nodiscard]] virtual int getFilteredContainerSize() const = 0;
    virtual void tick(int) = 0;
    [[nodiscard]] virtual ContainerWeakRef getContainerWeakRef() const = 0;
    [[nodiscard]] virtual const ItemStack &getItemStack(int model_slot) const = 0;
    [[nodiscard]] virtual const std::vector<ItemStack> &getItems() const = 0;
    [[nodiscard]] virtual const ItemInstance &getItemInstance(int model_slot) const = 0;
    [[nodiscard]] virtual const ItemStackBase &getItemStackBase(int model_slot) const = 0;
    [[nodiscard]] virtual bool isItemInstanceBased() const = 0;
    virtual void setItem(int model_slot, const ItemStack &item) = 0;
    [[nodiscard]] virtual bool isValid() = 0;
    [[nodiscard]] virtual bool isItemFiltered(const ItemStackBase &) const = 0;
    [[nodiscard]] virtual bool isExpanableItemFiltered(int index) const = 0;
    [[nodiscard]] virtual ContainerExpandStatus getItemExpandStatus(int item_id) const = 0;
    [[nodiscard]] virtual const std::string &getItemGroupName(int item_id) const = 0;
    virtual void switchItemExpando(int item_id) = 0;
    [[nodiscard]] virtual bool isSlotDisabled(int slot) const = 0;
    virtual void refreshContainer(bool full_refresh) = 0;
    [[nodiscard]] virtual Container *_getContainer() const = 0;
    [[nodiscard]] virtual int _getContainerOffset() const = 0;
    virtual void _init() = 0;
    virtual void _onItemChanged(int model_slot, const ItemStack &old_item, const ItemStack &new_item) = 0;
};
