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

enum class ItemStackRequestActionType : std::uint8_t {
    Take = 0,
    Place = 1,
    Swap = 2,
    Drop = 3,
    Destroy = 4,
    Consume = 5,
    Create = 6,
    PlaceInItemContainerDeprecated = 7,
    TakeFromItemContainerDeprecated = 8,
    ScreenLabTableCombine = 9,
    ScreenBeaconPayment = 10,
    ScreenHUDMineBlock = 11,
    CraftRecipe = 12,
    CraftRecipeAuto = 13,
    CraftCreative = 14,
    CraftRecipeOptional = 15,
    CraftRepairAndDisenchant = 16,
    CraftLoom = 17,
    CraftNonImplementedDeprecated = 18,
    CraftResultsDeprecated = 19,
};
