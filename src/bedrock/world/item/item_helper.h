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

enum class ItemUseMethod : int {
    Unknown = -1,
    EquipArmor = 0,
    Eat = 1,
    Attack = 2,
    Consume = 3,
    Throw = 4,
    Shoot = 5,
    Place = 6,
    FillBottle = 7,
    FillBucket = 8,
    PourBucket = 9,
    UseTool = 10,
    Interact = 11,
    Retrieved = 12,
    Dyed = 13,
    Traded = 14,
    BrushingCompleted = 15,
    OpenedVault = 16,
    _Count = 17,
};

enum class ItemAcquisitionMethod : int {
    Unknown = -1,
    None = 0,
    PickedUp = 1,
    Crafted = 2,
    TakenFromChest = 3,
    TakenFromEnderchest = 4,
    Bought = 5,
    Anvil = 6,
    Smelted = 7,
    Brewed = 8,
    Filled = 9,
    Trading = 10,
    Fishing = 11,
    Container = 13,
    Feeding = 14,
};

enum class ItemCommandVisibility : std::uint8_t {
    Visible = 0,
    Hidden = 1,
    DeferToLegacyBlock = 2,
};
