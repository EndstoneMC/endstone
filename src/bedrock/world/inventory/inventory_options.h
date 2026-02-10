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

enum class InventoryLayout : int {
    None = 0,
    InventoryOnly = 1,
    Default = 2,
    RecipeBookOnly = 3,
    Count = 4,
};

enum class InventoryLeftTabIndex : int {
    None = 0,
    RecipeConstruction = 1,
    RecipeEquipment = 2,
    RecipeItems = 3,
    RecipeNature = 4,
    RecipeSearch = 5,
    Survival = 6,
    Count = 7,
};

enum class InventoryRightTabIndex : int {
    None = 0,
    FullScreen = 1,
    Crafting = 2,
    Armor = 3,
    Count = 4,
};

enum class InventoryLeftTabPocketIndex : int {
    None = 0,
    Survival = 1,
    RecipeNature = 2,
    RecipeItems = 3,
    RecipeEquipment = 4,
    RecipeConstruction = 5,
    RecipeSearch = 6,
    Count = 7,
};

struct InventoryOptions {
    static constexpr auto DEFAULT_LEFT_INVENTORY_TAB = InventoryLeftTabIndex::RecipeSearch;
    static constexpr auto DEFAULT_RIGHT_INVENTORY_TAB = InventoryRightTabIndex::Crafting;

    InventoryOptions(InventoryLeftTabIndex, InventoryRightTabIndex, bool, InventoryLayout, InventoryLayout);
    InventoryLeftTabIndex left_inventory_tab;
    InventoryRightTabIndex right_inventory_tab;
    bool filtering;
    InventoryLayout layout_inv;
    InventoryLayout layout_craft;
};
static_assert(sizeof(InventoryOptions) == 20);
