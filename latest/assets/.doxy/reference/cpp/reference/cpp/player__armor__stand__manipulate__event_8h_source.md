

# File player\_armor\_stand\_manipulate\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_armor\_stand\_manipulate\_event.h**](player__armor__stand__manipulate__event_8h.md)

[Go to the documentation of this file](player__armor__stand__manipulate__event_8h.md)


```C++
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

#include <utility>

#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {

class PlayerArmorStandManipulateEvent final : public PlayerInteractActorEvent {
public:
    ENDSTONE_EVENT(PlayerArmorStandManipulateEvent);

    PlayerArmorStandManipulateEvent(const NotNull<Player> &player, const NotNull<Actor> &right_clicked,
                                    ItemStack armor_stand_item, ItemStack player_item, EquipmentSlot slot)
        : PlayerInteractActorEvent(player, right_clicked), armor_stand_item_(std::move(armor_stand_item)),
          player_item_(std::move(player_item)), slot_(slot)
    {
    }

    [[nodiscard]] const ItemStack &getArmorStandItem() const { return armor_stand_item_; }

    [[nodiscard]] const ItemStack &getPlayerItem() const { return player_item_; }

    [[nodiscard]] EquipmentSlot getSlot() const { return slot_; }

private:
    ItemStack armor_stand_item_;
    ItemStack player_item_;
    EquipmentSlot slot_;
};

}  // namespace endstone
```


