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

InventoryTransactionError InventoryTransaction::executeFull(Player &player, bool is_sender_authority) const
{
    // https://github.com/pmmp/PocketMine-MP/blob/a554d2/src/network/mcpe/handler/InGamePacketHandler.php#L390
    // dropping item should have exactly 2 sources and each source should have one action
    if (actions_.size() == 2 &&
        std::ranges::all_of(actions_, [](const auto &pair) { return pair.second.size() == 1; })) {

        const InventorySource *inventory_source = nullptr;
        const InventoryAction *inventory_action = nullptr;
        const InventorySource *world_source = nullptr;
        const InventoryAction *world_action = nullptr;

        for (const auto &[source, actions] : actions_) {
            const auto &action = actions.at(0);
            if (source.getType() == InventorySourceType::ContainerInventory) {
                inventory_action = &action;
                inventory_source = &source;
            }
            else if (source.getType() == InventorySourceType::WorldInteraction) {
                world_action = &action;
                world_source = &source;
            }
        }

        if (world_source && world_action && inventory_source && inventory_action) {
            auto result = getVerifyFunction(*world_source)(player, *world_action, is_sender_authority);
            if (result != InventoryTransactionError::NoError) {
                return result;
            }
            result = getVerifyFunction(*inventory_source)(player, *inventory_action, is_sender_authority);
            if (result != InventoryTransactionError::NoError) {
                return result;
            }
            result = getExecuteFunction(*world_source)(player, *world_action);
            if (result != InventoryTransactionError::NoError) {
                return result;
            }
            result = getExecuteFunction(*inventory_source)(player, *inventory_action);
            return result;
        }
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL(&InventoryTransaction::executeFull, this, player, is_sender_authority);
}

std::function<InventoryTransactionError(Player &, const InventoryAction &, bool)> InventoryTransaction::
    getVerifyFunction(const InventorySource &source) const
{
    switch (source.getType()) {
    case InventorySourceType::ContainerInventory:
        return
            [](Player &player, const InventoryAction &action, bool is_sender_authority) -> InventoryTransactionError {
                // Determine container size (only inventory is supported here)
                int container_size = 0;
                if (action.getSource().getContainerId() == CONTAINER_ID_INVENTORY) {
                    container_size = player.getInventory().getContainerSize();
                }

                // Default to size‐mismatch if the slot is out of bounds
                auto result = InventoryTransactionError::SizeMismatch;

                // If slot is valid, load the item and check it
                if (action.getSlot() < container_size) {
                    // Pull the source item (only from inventory)
                    const auto &server_item = action.getSource().getContainerId() == CONTAINER_ID_INVENTORY
                                                ? player.getInventory().getItem(static_cast<int>(action.getSlot()))
                                                : ItemStack::EMPTY_ITEM;

                    // If it matches, or the sender has authority, it's OK
                    if (checkTransactionItemsMatch(server_item, action.getFromItem()) || is_sender_authority) {
                        result = InventoryTransactionError::NoError;
                    }
                    else {
                        // Otherwise, it’s a source‐item mismatch
                        result = InventoryTransactionError::SourceItemMismatch;
                    }
                }
                return result;
            };
    case InventorySourceType::WorldInteraction:
        return [](Player & /*player*/, const InventoryAction &action,
                  bool is_sender_authority) -> InventoryTransactionError {
            auto result = InventoryTransactionError::Unknown;
            // Slot 0: Drop
            if (action.getSlot() == 0 && !action.getFromItem() && action.getToItem()) {
                result = InventoryTransactionError::NoError;
            }
            // Slot 1: Pick up
            else if (action.getSlot() == 1 && action.getFromItem() && !action.getToItem()) {
                result = is_sender_authority ? InventoryTransactionError::NoError
                                             : InventoryTransactionError::AuthorityMismatch;
            }
            return result;
        };
    default:
        throw std::runtime_error("Unsupported InventorySource type");
    }
}

std::function<InventoryTransactionError(Player &, const InventoryAction &)> InventoryTransaction::getExecuteFunction(
    const InventorySource &source) const
{
    switch (source.getType()) {
    case InventorySourceType::ContainerInventory:
        return [](Player &player, const InventoryAction &action) -> InventoryTransactionError {
            if (action.getSource().getContainerId() == CONTAINER_ID_INVENTORY) {
                player.getInventory().setItemWithForceBalance(static_cast<int>(action.getSlot()), action.getToItem(),
                                                              false);
            }
            return InventoryTransactionError::NoError;
        };
    case InventorySourceType::WorldInteraction:
        return [](Player &player, const InventoryAction &action) -> InventoryTransactionError {
            auto result = InventoryTransactionError::Unknown;
            // Slot 0: Drop
            if (action.getSlot() == 0 && !action.getFromItem() && action.getToItem()) {
                auto randomly = (action.getSource().getFlags() & InventorySource::WorldInteraction_Random) != 0;
                if (player.drop(action.getToItem(), randomly)) {
                    result = InventoryTransactionError::NoError;
                }
            }
            // Slot 1: Pick up
            else if (action.getSlot() == 1 && action.getFromItem() && !action.getToItem()) {
                result = InventoryTransactionError::NoError;
            }
            return result;
        };
    default:
        throw std::runtime_error("Unsupported InventorySource type");
    }
}

bool InventoryTransaction::checkTransactionItemsMatch(const ItemStack &server_item, const ItemStack &client_item)
{
    ItemStack client_item_copy(client_item);
    if (!client_item_copy.isBlock()) {
        client_item_copy.setAuxValue(server_item.getAuxValue());
        if (server_item.hasDamageValue()) {
            client_item_copy.setDamageValue(server_item.getDamageValue());
        }
        else {
            client_item_copy.removeDamageValue();
        }
    }
    return client_item_copy == server_item;
}
