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

/** Identifies an action in an ItemStackRequest. */
enum class ItemStackRequestActionType : std::uint8_t {
    Take = 0,
    Place = 1,
    Swap = 2,
    Drop = 3,
    Destroy = 4,
    CraftingConsumeInput = 5,
    CraftingCreateSpecificResult = 6,
    LabTableCombine = 9,
    BeaconPayment = 10,
    MineBlock = 11,
    CraftingRecipe = 12,
    CraftingRecipeAuto = 13,
    CreativeCreate = 14,
    CraftingRecipeOptional = 15,
    CraftingGrindstone = 16,
    CraftingLoom = 17,
    CraftingNonImplementedDeprecated = 18,
    CraftingResultsDeprecated = 19,
};
