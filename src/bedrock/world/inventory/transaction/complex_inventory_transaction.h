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

#include "bedrock/platform/result.h"
#include "bedrock/world/inventory/transaction/inventory_transaction.h"

class BinaryStream;
class BlockPalette;
class Player;
class ReadOnlyBinaryStream;

class ComplexInventoryTransaction {
public:
    enum class Type : std::uint32_t {
        NormalTransaction = 0,
        InventoryMismatch = 1,
        ItemUseTransaction = 2,
        ItemUseOnEntityTransaction = 3,
        ItemReleaseTransaction = 4,
    };

    virtual ~ComplexInventoryTransaction() = default;
    virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &)
    {
        return {};
    }
    virtual void write(BinaryStream &) const {}
    virtual void postLoadItems(BlockPalette &, bool) {}
    virtual InventoryTransactionError handle(Player &, bool) const
    {
        return {};
    }
    virtual void onTransactionError(Player &, InventoryTransactionError) const {}

    Type type_;                        // +8
    InventoryTransaction transaction_;  // +16
};
