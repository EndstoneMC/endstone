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

#include "endstone/event/event.h"
#include "endstone/inventory/inventory.h"

namespace endstone {

/**
 * Represents a player related inventory event.
 */
class InventoryEvent : public Event {
public:
    explicit InventoryEvent(Inventory &inventory) : inventory_(inventory) {};

    /**
     * Gets the primary Inventory involved in this transaction.
     *
     * @return The primary inventory
     */
    [[nodiscard]] Inventory &getInventory() const { return inventory_; }

private:
    Inventory &inventory_;
};

}  // namespace endstone
