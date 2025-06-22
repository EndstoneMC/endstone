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

#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/world/inventory/transaction/inventory_action.h"
#include "bedrock/world/inventory/transaction/inventory_source.h"
#include "bedrock/world/inventory/transaction/inventory_transaction_group.h"

class Player;

enum class InventoryTransactionError : int {
    Unknown = 0,
    NoError = 1,
    BalanceMismatch = 2,
    SourceItemMismatch = 3,
    InventoryMismatch = 4,
    SizeMismatch = 5,
    AuthorityMismatch = 6,
    StateMismatch = 7,
    ApiDenied = 8,
};

class InventoryTransaction {
public:
    ENDSTONE_HOOK InventoryTransactionError executeFull(Player &player, bool is_sender_authority) const;
    [[nodiscard]] std::function<InventoryTransactionError(Player &, const InventoryAction &, bool)> getVerifyFunction(
        const InventorySource &source) const;
    [[nodiscard]] std::function<InventoryTransactionError(Player &, const InventoryAction &)> getExecuteFunction(
        const InventorySource &source) const;
    static bool checkTransactionItemsMatch(const ItemStack & server_item, const ItemStack & client_item);

private:
    std::unordered_map<InventorySource, std::vector<InventoryAction>> actions_;
    std::vector<InventoryTransactionItemGroup> contents_;
};
