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

#include "bedrock/world/actor/player/player_inventory.h"

int PlayerInventory::getHotbarSize() const
{
    return inventory_->getHotbarSize();
}

PlayerInventorySlotData PlayerInventory::getSelectedSlot() const
{
    return PlayerInventorySlotData(selected_, selected_container_id_);
}

ContainerID PlayerInventory::getSelectedContainerId()
{
    return selected_container_id_;
}

bool PlayerInventory::selectSlot(const int slot, const ContainerID container_id)
{
    if (slot < 0 || slot >= getHotbarSize()) {
        return false;
    }
    selected_ = slot;
    selected_container_id_ = container_id;
    return true;
}

const Container &PlayerInventory::getContainer() const
{
    return *inventory_;
}

Container &PlayerInventory::getContainer()
{
    return *inventory_;
}
