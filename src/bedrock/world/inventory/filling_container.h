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

#include "bedrock/world/container.h"
#include "bedrock/world/item/save_context.h"

class FillingContainer : public Container {
protected:
    using ItemList = std::vector<ItemStack>;

public:
    static constexpr int MAX_INVENTORY_STACK_SIZE = 254;
    static constexpr int LEGACY_INVENTORY_SIZE = 54;
    static constexpr int SURVIVAL_INVENTORY_SIZE = 36;
    static constexpr int HOTBAR_SIZE = 9;

    FillingContainer(Player *, int, ContainerType);
    ~FillingContainer() override;
    virtual int removeResource(const ItemStack &, bool, bool, int);
    virtual void swapSlots(int, int);
    virtual bool add(ItemStack &);
    virtual bool canAdd(const ItemStack &) const;
    void removeItem(int, int) override;
    virtual void clearSlot(int);
    virtual int clearInventory(int);
    virtual std::unique_ptr<ListTag> saveToTag(const SaveContext &) const;
    virtual void loadFromTag(const ListTag &);
    void setItem(int, const ItemStack &) override;
    void setItemWithForceBalance(int, const ItemStack &, bool) override;
    const ItemStack &getItem(int) const override;
    int getMaxStackSize() const override;
    int getContainerSize() const override;
    void startOpen(Actor &) override;
    void serverInitItemStackIds(int, int, ItemStackNetIdChangedCallback) override;

    [[nodiscard]] int getHotbarSize() const { return HOTBAR_SIZE; }

private:
    virtual void _trySetInSlot(ItemStack &, const int &, const int &, int &);

protected:
    ItemList items_;
    Player *player_;
};
