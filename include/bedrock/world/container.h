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

#include <cstdint>
#include <functional>

#include "bedrock/core/math/vec3.h"
#include "bedrock/forward.h"
#include "bedrock/world/item/item_stack.h"

enum class ContainerType : std::int8_t {
    NONE = -9,
    INVENTORY = -1,
    CONTAINER = 0,
    WORKBENCH = 1,
    FURNACE = 2,
    ENCHANTMENT = 3,
    BREWING_STAND = 4,
    ANVIL = 5,
    DISPENSER = 6,
    DROPPER = 7,
    HOPPER = 8,
    CAULDRON = 9,
    MINECART_CHEST = 10,
    MINECART_HOPPER = 11,
    HORSE = 12,
    BEACON = 13,
    STRUCTURE_EDITOR = 14,
    TRADE = 15,
    COMMAND_BLOCK = 16,
    JUKEBOX = 17,
    ARMOR = 18,
    HAND = 19,
    COMPOUND_CREATOR = 20,
    ELEMENT_CONSTRUCTOR = 21,
    MATERIAL_REDUCER = 22,
    LAB_TABLE = 23,
    LOOM = 24,
    LECTERN = 25,
    GRINDSTONE = 26,
    BLAST_FURNACE = 27,
    SMOKER = 28,
    STONECUTTER = 29,
    CARTOGRAPHY = 30,
    HUD = 31,
    JIGSAW_EDITOR = 32,
    SMITHING_TABLE = 33,
    CHEST_BOAT = 34,
    DECORATED_POT = 35,
    CRAFTER = 36,
};

class Container {
    using ItemStackNetIdChangedCallback = std::function<void(int, const ItemStack &)>;

public:
    virtual ~Container() = 0;
    virtual void init() = 0;
    virtual void serverInitItemStackIds(int, int, ItemStackNetIdChangedCallback) = 0;
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
    virtual void startOpen(Player &) = 0;
    virtual void stopOpen(Player &player);
    [[nodiscard]] virtual std::vector<ItemStack> getSlotCopies() const;
    [[nodiscard]] virtual std::vector<const ItemStack *> const getSlots() const;
    [[nodiscard]] virtual int getEmptySlotsCount() const;
    [[nodiscard]] virtual int getItemCount(ItemStack const &compare) const;
    [[nodiscard]] virtual int findFirstSlotForItem(ItemStack const &item) const;
    [[nodiscard]] virtual bool canPushInItem(int, int, ItemStack const &) const;
    [[nodiscard]] virtual bool canPullOutItem(int, int, ItemStack const &) const;
    virtual void setContainerChanged(int slot);
    virtual void setContainerMoved();
    virtual void setCustomName(std::string const &name);
    [[nodiscard]] virtual bool hasCustomName() const;
    virtual void readAdditionalSaveData(CompoundTag const &tag);
    virtual void addAdditionalSaveData(CompoundTag &tag);
    virtual void createTransactionContext(
        std::function<void(Container &, int, const ItemStack &, const ItemStack &)> callback,
        std::function<void()> execute);
    virtual void initializeContainerContents(BlockSource &region);
    [[nodiscard]] virtual bool isEmpty() const;
    [[nodiscard]] virtual bool isSlotDisabled(int) const;
};
