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

#include <optional>
#include <string>
#include <vector>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/item/crafting/recipe.h"
#include "bedrock/world/item/crafting/recipes.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/item/crafting/recipe_ingredient.h"
#include "bedrock/world/item/crafting/recipe_unlocking_requirement.h"
#include "bedrock/world/item/network_item_instance_descriptor_data.h"

struct ShapelessRecipePayload {
    std::string recipe_id;                                          // +0
    std::vector<RecipeIngredient> ingredients;                      // +32
    std::vector<NetworkItemInstanceDescriptorData> results;         // +56
    mce::UUID uuid;                                                 // +80
    std::string tag;                                                // +96
    int priority;                                                   // +128
    std::optional<RecipeUnlockingRequirement> unlocking_requirement;  // +136
    RecipeNetId net_id;                                             // +184
};
BEDROCK_STATIC_ASSERT_SIZE(ShapelessRecipePayload, 192, 176);

struct ShapedRecipePayload {
    std::string recipe_id;                                          // +0
    int width;                                                      // +32
    int height;                                                     // +36
    std::vector<RecipeIngredient> ingredients;                      // +40, width * height, row-major
    std::vector<NetworkItemInstanceDescriptorData> results;         // +64
    mce::UUID uuid;                                                 // +88
    std::string tag;                                                // +104
    int priority;                                                   // +136
    bool assume_symmetry;                                           // +140
    std::optional<RecipeUnlockingRequirement> unlocking_requirement;  // +144
    RecipeNetId net_id;                                             // +192
};
BEDROCK_STATIC_ASSERT_SIZE(ShapedRecipePayload, 200, 184);

struct MultiRecipePayload {
    mce::UUID uuid;    // +0
    RecipeNetId net_id;  // +16
};
BEDROCK_STATIC_ASSERT_SIZE(MultiRecipePayload, 24, 24);

struct SmithingTransformRecipePayload {
    std::string recipe_id;                        // +0
    RecipeIngredient template_ingredient;         // +32
    RecipeIngredient base_ingredient;             // +56
    RecipeIngredient addition_ingredient;         // +80
    NetworkItemInstanceDescriptorData result;     // +104
    std::string tag;                              // +152
    RecipeNetId net_id;                           // +184
};
BEDROCK_STATIC_ASSERT_SIZE(SmithingTransformRecipePayload, 192, 168);

struct SmithingTrimRecipePayload {
    std::string recipe_id;                 // +0
    RecipeIngredient template_ingredient;  // +32
    RecipeIngredient base_ingredient;      // +56
    RecipeIngredient addition_ingredient;  // +80
    std::string tag;                       // +104
    RecipeNetId net_id;                    // +136
};
BEDROCK_STATIC_ASSERT_SIZE(SmithingTrimRecipePayload, 144, 128);

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

struct CraftingDataPacketPayload {
    std::vector<ShapedRecipePayload> shaped_recipes;                        // +0
    std::vector<ShapelessRecipePayload> shapeless_recipes;                  // +24
    std::vector<MultiRecipePayload> multi_recipes;                          // +48
    std::vector<ShapelessRecipePayload> user_data_shapeless_recipes;        // +72
    std::vector<ShapelessRecipePayload> shapeless_chemistry_recipes;        // +96
    std::vector<ShapedRecipePayload> shaped_chemistry_recipes;              // +120
    std::vector<SmithingTransformRecipePayload> smithing_transform_recipes;  // +144
    std::vector<SmithingTrimRecipePayload> smithing_trim_recipes;           // +168
    std::vector<PotionMixDataEntry> potion_mixes;                           // +192
    std::vector<ContainerMixDataEntry> container_mixes;                     // +216
    std::vector<MaterialReducerDataEntry> material_reducers;                // +240, not filled by prepareFromRecipes
    bool clear_recipes;                                                     // +264
};
BEDROCK_STATIC_ASSERT_SIZE(CraftingDataPacketPayload, 272, 272);

class CraftingDataPacket : public Packet {
public:
    ~CraftingDataPacket() override = default;

    static CraftingDataPacketPayload prepareFromRecipes(Recipes const &, bool);

    CraftingDataPacketPayload payload;                                      // +48
    SerializationMode serialization_mode{SerializationMode::CerealOnly};    // +320
};
BEDROCK_STATIC_ASSERT_SIZE(CraftingDataPacket, 328, 328);
