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
#include <optional>
#include <string>
#include <string_view>

namespace ContainerCollectionNames {
inline constexpr std::string_view EnchantingInputContainer = "enchanting_input_items";
}  // namespace ContainerCollectionNames

enum class ContainerEnumName : std::uint8_t {
    AnvilInputContainer = 0,
    AnvilMaterialContainer = 1,
    AnvilResultPreviewContainer = 2,
    SmithingTableInputContainer = 3,
    SmithingTableMaterialContainer = 4,
    SmithingTableResultPreviewContainer = 5,
    ArmorContainer = 6,
    LevelEntityContainer = 7,
    BeaconPaymentContainer = 8,
    BrewingStandInputContainer = 9,
    BrewingStandResultContainer = 10,
    BrewingStandFuelContainer = 11,
    CombinedHotbarAndInventoryContainer = 12,
    CraftingInputContainer = 13,
    CraftingOutputPreviewContainer = 14,
    RecipeConstructionContainer = 15,
    RecipeNatureContainer = 16,
    RecipeItemsContainer = 17,
    RecipeSearchContainer = 18,
    RecipeSearchBarContainer = 19,
    RecipeEquipmentContainer = 20,
    RecipeBookContainer = 21,
    EnchantingInputContainer = 22,
    EnchantingMaterialContainer = 23,
    FurnaceFuelContainer = 24,
    FurnaceIngredientContainer = 25,
    FurnaceResultContainer = 26,
    HorseEquipContainer = 27,
    HotbarContainer = 28,
    InventoryContainer = 29,
    ShulkerBoxContainer = 30,
    TradeIngredient1Container = 31,
    TradeIngredient2Container = 32,
    TradeResultPreviewContainer = 33,
    OffhandContainer = 34,
    CompoundCreatorInput = 35,
    CompoundCreatorOutputPreview = 36,
    ElementConstructorOutputPreview = 37,
    MaterialReducerInput = 38,
    MaterialReducerOutput = 39,
    LabTableInput = 40,
    LoomInputContainer = 41,
    LoomDyeContainer = 42,
    LoomMaterialContainer = 43,
    LoomResultPreviewContainer = 44,
    BlastFurnaceIngredientContainer = 45,
    SmokerIngredientContainer = 46,
    Trade2Ingredient1Container = 47,
    Trade2Ingredient2Container = 48,
    Trade2ResultPreviewContainer = 49,
    GrindstoneInputContainer = 50,
    GrindstoneAdditionalContainer = 51,
    GrindstoneResultPreviewContainer = 52,
    StonecutterInputContainer = 53,
    StonecutterResultPreviewContainer = 54,
    CartographyInputContainer = 55,
    CartographyAdditionalContainer = 56,
    CartographyResultPreviewContainer = 57,
    BarrelContainer = 58,
    CursorContainer = 59,
    CreatedOutputContainer = 60,
    SmithingTableTemplateContainer = 61,
    CrafterLevelEntityContainer = 62,
    DynamicContainer = 63,
    RecipeFoodContainer = 64,
    RecipeBlocksContainer = 65,
    RecipeFurnaceItemsContainer = 66,
};

using ContainerNameDynamicID = std::uint32_t;

struct FullContainerName {
    [[nodiscard]] bool isDynamic() const;
    bool operator==(const FullContainerName &) const;
    bool operator!=(const FullContainerName &) const;
    [[nodiscard]] std::string toString() const;

    ContainerEnumName name;
    std::optional<ContainerNameDynamicID> dynamic_id;
};
static_assert(sizeof(FullContainerName) == 12);
