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

#include "bedrock/world/inventory/transaction/inventory_transaction.h"

#include "bedrock/world/actor/player/player.h"
#include "endstone/runtime/hook.h"

/*
{
    InventorySource(type=WorldInteraction, container_id=CONTAINER_ID_NONE, flags=0): [
        InventoryAction(source=InventorySource(type=WorldInteraction, container_id=CONTAINER_ID_NONE, flags=0),
                        slot=0,
                        from=[ItemStackNetId: [TypedServerNetId: 0]] 0 x (0)@0,
                        to=[ItemStackNetId: [TypedServerNetId: 0]] 1 x Rotten Flesh(299)@0
        )
    ],
    InventorySource(type=ContainerInventory, container_id=CONTAINER_ID_INVENTORY, flags=0): [
        InventoryAction(source=InventorySource(type=ContainerInventory, container_id=CONTAINER_ID_INVENTORY, flags=0),
                        slot=2,
                        from=[ItemStackNetId: [TypedServerNetId: 0]] 1 x Rotten Flesh(299)@0,
                        to=[ItemStackNetId: [TypedServerNetId: 0]] 0 x (0)@0
        )
    ]
}
*/

InventoryTransactionError InventoryTransaction::executeWorldInteraction(void *, Player &player,
                                                                        const InventoryAction &action)
{
    auto result = InventoryTransactionError::Unknown;
    // Slot 0: Drop
    if (action.getSlot() == 0 && !action.getFromItem() && action.getToItem()) {
        auto randomly = (action.getSource().getFlags() & InventorySource::WorldInteraction_Random) != 0;
        // #blameMojang: The original implementation ignores the return value of Player::drop,
        // unconditionally marking the transaction as successful. This means even when drop is
        // cancelled (e.g. by a plugin via PlayerDropItemEvent), the server still considers the
        // transaction valid and the item disappears from the inventory. Our fix here only marks
        // the transaction as successful if Player::drop returns true; otherwise we return an
        // error so the client reverts the transaction and the item stays in the inventory.
        if (player.drop(action.getToItem(), randomly)) {
            result = InventoryTransactionError::NoError;
        }
    }
    // Slot 1: Pick up
    else if (action.getSlot() == 1 && action.getFromItem() && !action.getToItem()) {
        result = InventoryTransactionError::NoError;
    }
    return result;
}
