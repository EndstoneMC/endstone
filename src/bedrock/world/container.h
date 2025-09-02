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
#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/container_runtime_id.h"
#include "bedrock/world/item/item_stack.h"

class ContainerSizeChangeListener {
public:
    virtual void containerSizeChanged(int) = 0;
    virtual ~ContainerSizeChangeListener();
};
static_assert(sizeof(ContainerSizeChangeListener) == 8);

class ContainerContentChangeListener {
public:
    virtual void containerContentChanged(int) = 0;
    virtual ~ContainerContentChangeListener();
};
static_assert(sizeof(ContainerContentChangeListener) == 8);

class ContainerCloseListener {
public:
    virtual void containerClosed(Actor &) = 0;
    virtual ~ContainerCloseListener();
};
static_assert(sizeof(ContainerCloseListener) == 8);

struct ContainerOwner {
    class OwnedContainer {
    public:
        OwnedContainer(std::shared_ptr<ContainerModel>);

    private:
        std::shared_ptr<ContainerModel> container_model_;
    };
    static_assert(sizeof(OwnedContainer) == 16);
    std::vector<OwnedContainer> owned_containers;
};
static_assert(sizeof(ContainerOwner) == 24);

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
public:
    struct PublisherWrapper {
        Bedrock::PubSub::Publisher<void(), Bedrock::PubSub::ThreadModel::SingleThreaded> publisher;
    };

    using ItemStackNetIdChangedCallback = std::function<void(int, const ItemStack &)>;

    static constexpr int LARGE_MAX_STACK_SIZE = 64;
    static constexpr int DEFAULT_CONTAINER_SIZE = 27;
    static constexpr int INVALID_SLOT = -1;
    static constexpr std::uint32_t INVALID_CONTAINER_ID = 0;

    Container(ContainerType);
    Container(ContainerType, const std::string &, bool);

    virtual ~Container() = default;
    virtual void init();
    virtual void serverInitItemStackIds(int, int, ItemStackNetIdChangedCallback) = 0;
    virtual void addContentChangeListener(ContainerContentChangeListener *);
    virtual void removeContentChangeListener(ContainerContentChangeListener *);
    virtual Bedrock::PubSub::Connector<void()> *getContainerRemovedConnector();
    [[nodiscard]] virtual bool hasRemovedSubscribers() const;
    [[nodiscard]] virtual ItemStack const &getItem(int) const = 0;
    virtual bool hasRoomForItem(ItemStack const &);
    virtual bool addItem(ItemStack &);
    virtual bool addItemWithForceBalance(ItemStack &);
    virtual bool addItemToFirstEmptySlot(ItemStack const &);
    virtual void setItem(int, ItemStack const &) = 0;
    virtual void setItemWithForceBalance(int, ItemStack const &, bool);
    virtual void removeItem(int, int);
    virtual void removeAllItems();
    virtual void removeAllItemsWithForceBalance();
    virtual void containerRemoved();
    virtual void dropSlotContent(BlockSource &, Vec3 const &, bool, int);
    virtual void dropContents(BlockSource &, Vec3 const &, bool);
    [[nodiscard]] virtual int getContainerSize() const = 0;
    [[nodiscard]] virtual int getMaxStackSize() const = 0;
    virtual void startOpen(Actor &) = 0;
    virtual void stopOpen(Actor &);
    [[nodiscard]] virtual std::vector<ItemStack> getSlotCopies() const;
    [[nodiscard]] virtual std::vector<const ItemStack *> const getSlots() const;
    [[nodiscard]] virtual int getEmptySlotsCount() const;
    [[nodiscard]] virtual int getItemCount(const ItemStack &) const;
    [[nodiscard]] virtual int firstEmptySlot() const;
    [[nodiscard]] virtual int firstItem() const;
    [[nodiscard]] virtual int findFirstSlotForItem(ItemStack const &item) const;
    [[nodiscard]] virtual int reverseFindFirstSlotForItem(const ItemStack &) const;
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

protected:
    ContainerType container_type_;
    ContainerType gameplay_container_type_;
    std::unordered_set<ContainerContentChangeListener *> content_change_listeners_;
    std::unordered_set<ContainerSizeChangeListener *> size_change_listeners_;
    std::unordered_set<ContainerCloseListener *> close_listeners_;
    PublisherWrapper removed_publisher_;
    std::deque<std::function<void(Container &, int, const ItemStack &, const ItemStack &)>> transaction_context_stack_;
    Bedrock::Safety::RedactableString name_;
    bool custom_name_;
    ContainerOwner container_owner_;

private:
    ContainerRuntimeId container_runtime_id_;
};
