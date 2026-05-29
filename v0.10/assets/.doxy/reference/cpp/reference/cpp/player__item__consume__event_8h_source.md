

# File player\_item\_consume\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_item\_consume\_event.h**](player__item__consume__event_8h.md)

[Go to the documentation of this file](player__item__consume__event_8h.md)


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

#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"

namespace endstone {
class PlayerItemConsumeEvent final : public Cancellable<PlayerEvent> {
public:
    ENDSTONE_EVENT(PlayerItemConsumeEvent)

    explicit PlayerItemConsumeEvent(Player &player, const ItemStack &item, EquipmentSlot hand)
        : Cancellable(player), item_(item), hand_(hand)
    {
    }

    [[nodiscard]] std::unique_ptr<ItemStack> getItem() const
    {
        return item_.clone();
    }

    [[nodiscard]] EquipmentSlot getHand() const
    {
        return hand_;
    }

private:
    const ItemStack &item_;
    EquipmentSlot hand_;
};
}  // namespace endstone
```


