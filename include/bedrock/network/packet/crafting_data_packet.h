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

#include <string>
#include <vector>

#include "bedrock/core/hashed_string.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "bedrock/world/item/network_item_instance_descriptor.h"

enum CraftingDataEntryType {
    ShapelessRecipe = 0,
    ShapedRecipe = 1,
    FurnaceRecipe = 2,
    FurnaceAuxRecipe = 3,
    MultiRecipe = 4,
    ShulkerBoxRecipe = 5,
    ShapelessChemistryRecipe = 6,
    ShapedChemistryRecipe = 7,
    SmithingTransformRecipe = 8,
    SmithingTrimRecipe = 9,
    COUNT = 10,
};

struct CraftingDataEntry {
    std::unique_ptr<Recipe> recipe;
    int item_data;
    int item_aux;
    HashedString tag;
    NetworkItemInstanceDescriptor item_result;
    CraftingDataEntryType entry_type;
};

struct PotionMixDataEntry {
    int from_item_id;
    int from_item_aux;
    int reagent_item_id;
    int reagent_item_aux;
    int to_item_id;
    int to_item_aux;
};

struct ContainerMixDataEntry {
    int from_item_id;
    int reagent_item_id;
    int to_item_id;
};

struct MaterialReducerEntryOutput {
    int to_item_id;
    int to_item_count;
};

struct MaterialReducerDataEntry {
    int from_item_key;
    std::vector<MaterialReducerEntryOutput> to_item_ids_and_counts;
};

class CraftingDataPacket : public Packet {
public:
    ~CraftingDataPacket() override = default;

    ENDSTONE_HOOK static std::unique_ptr<CraftingDataPacket> prepareFromRecipes(Recipes const &, bool);

    std::vector<CraftingDataEntry> crafting_entries;                 // +48
    std::vector<PotionMixDataEntry> potion_mix_entries;              // +72
    std::vector<ContainerMixDataEntry> container_mix_entries;        // +96
    std::vector<MaterialReducerDataEntry> material_reducer_entries;  // +120
    bool clear_recipes;                                              // +144
};
