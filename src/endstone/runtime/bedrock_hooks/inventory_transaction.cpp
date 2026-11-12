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
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"
#include "endstone/runtime/hook.h"

#include <utility>
#include <vector>

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

namespace {
thread_local int execute_depth = 0;
thread_local std::vector<std::pair<int, ItemStack>> pending_restores;

void restore(Player &player, int slot, const ItemStack &drop)
{
    auto &container = player.getInventory();
    const auto &current = container.getItem(slot);
    if (current.isNull()) {
        container.setItemWithForceBalance(slot, drop, true);
    }
    else if (current.matchesItem(drop) && current.getCount() + drop.getCount() <= current.getMaxStackSize()) {
        auto restored = current;
        restored.set(current.getCount() + drop.getCount());
        container.setItemWithForceBalance(slot, restored, true);
    }
    else {
        auto leftover = drop;
        container.addItemWithForceBalance(leftover);
    }
}
}  // namespace

InventoryTransactionError InventoryTransaction::executeFull(Player &player, bool is_sender_authority) const
{
    ++execute_depth;
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&InventoryTransaction::executeFull, this, player, is_sender_authority);
    if (--execute_depth == 0) {
        auto restores = std::move(pending_restores);
        pending_restores.clear();
        if (result == InventoryTransactionError::NoError) {
            for (const auto &[slot, drop] : restores) {
                restore(player, slot, drop);
            }
        }
    }
    return result;
}

InventoryTransactionError InventoryTransaction::executeWorldInteraction(void *, Player &player,
                                                                        const InventoryAction &action)
{
    auto result = InventoryTransactionError::Unknown;
    // Slot 0: Drop
    if (action.getSlot() == 0 && !action.getFromItem() && action.getToItem()) {
        const auto &drop = action.getToItem();
        auto randomly = (action.getSource().getFlags() & InventorySource::WorldInteraction_Random) != 0;
        // #blameMojang: ContainerInventory may run before or after this action, so restore once executeFull is done
        if (!player.drop(drop, randomly)) {
            const auto slot = player.getSelectedItemSlot();
            if (execute_depth > 0) {
                pending_restores.emplace_back(slot, drop);
            }
            else {
                restore(player, slot, drop);
            }
        }
        result = InventoryTransactionError::NoError;
    }
    // Slot 1: Pick up
    else if (action.getSlot() == 1 && action.getFromItem() && !action.getToItem()) {
        result = InventoryTransactionError::NoError;
    }
    return result;
}
