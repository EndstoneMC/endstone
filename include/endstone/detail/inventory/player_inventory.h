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

#include "endstone/detail/inventory/inventory.h"
#include "endstone/inventory/player_inventory.h"

namespace endstone::detail {

class EndstonePlayerInventory : public EndstoneInventory, public PlayerInventory {
public:
    using EndstoneInventory::EndstoneInventory;

    int getSize() const override
    {
        return EndstoneInventory::getSize();
    }

    int getMaxStackSize() const override
    {
        return EndstoneInventory::getMaxStackSize();
    }
};

}  // namespace endstone::detail
