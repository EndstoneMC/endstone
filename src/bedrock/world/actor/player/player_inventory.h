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

#include "bedrock/world/actor/player/inventory.h"
#include "bedrock/world/actor/player/player_inventory_slot_data.h"
#include "bedrock/world/container.h"
#include "bedrock/world/container_id.h"
#include "bedrock/world/item/item_stack.h"

class PlayerInventory : public ContainerSizeChangeListener, ContainerContentChangeListener {
public:
    PlayerInventory(std::unique_ptr<Inventory>);
    ~PlayerInventory() override = 0;
    void containerSizeChanged(int) override = 0;
    void containerContentChanged(int) override = 0;
    virtual void createTransactionContext(std::function<void(Container &, int, const ItemStack &, const ItemStack &)>,
                                          std::function<void()>);

    [[nodiscard]] int getHotbarSize() const;
    [[nodiscard]] PlayerInventorySlotData getSelectedSlot() const;
    [[nodiscard]] ContainerID getSelectedContainerId();
    bool selectSlot(int slot, ContainerID container_id);
    [[nodiscard]] const Container &getContainer() const;
    Container &getContainer();

private:
    static bool isHotbarSlot(int slot);

    int selected_;
    ItemStack infinite_item_;
    ContainerID selected_container_id_;
    std::unique_ptr<Inventory> inventory_;
    std::vector<ItemStack> complex_items_;
    std::weak_ptr<HudContainerManagerModel> hud_container_manager_;
};
