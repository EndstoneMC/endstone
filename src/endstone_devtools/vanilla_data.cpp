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

#include "endstone/detail/devtools/vanilla_data.h"

#include <magic_enum/magic_enum.hpp>

#include "bedrock/nbt/nbt_io.h"
#include "bedrock/network/packet/crafting_data_packet.h"
#include "bedrock/util/string_byte_output.h"
#include "bedrock/world/item/registry/creative_item_registry.h"
#include "bedrock/world/level/block/actor/furnace_block_actor.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/detail/base64.h"
#include "endstone/detail/devtools/imgui/imgui_json.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"
#include "endstone/detail/util/uuid.h"

namespace endstone::detail::devtools {

namespace {
double round(double d)
{
    d *= 1000000.0;
    if (d >= 0) {
        d += 0.5;
    }
    else {
        d -= 0.5;
    }
    return static_cast<int>(d) / 1000000.0;
}

void dumpBlockData(VanillaData &data, ::Level &level)
{
    auto overworld = level.getDimension(VanillaDimensions::Overworld);
    auto &region = overworld->getBlockSourceFromMainChunkSource();
    auto item_registry = level.getItemRegistry();

    BlockTypeRegistry::forEachBlock([&](const BlockLegacy &block_legacy) {
        const auto &name = block_legacy.getFullNameId();

        nlohmann::json tags;
        for (const auto &tag : block_legacy.getTags()) {
            auto tag_name = tag.getString();
            if (tag_name.rfind("minecraft:", 0) == std::string::npos) {
                tag_name = "minecraft:" + tag_name;
            }
            tags.push_back(tag_name);

            if (!data.block_tags.contains(tag_name)) {
                data.block_tags[tag_name] = {};
            }
            data.block_tags[tag_name].push_back(name);
        }

        data.block_types[name] = {{"defaultBlockStateHash", block_legacy.getDefaultState()->getRuntimeId()}};
        if (!tags.is_null()) {
            data.block_types[name]["tags"] = tags;
        }

        block_legacy.forEachBlockPermutation([&](const ::Block &block) {
            AABB collision_shape;
            AABB outline_shape;
            AABB visual_shape;
            AABB ui_shape;
            AABB liquid_clip_shape;
            block.getCollisionShape(collision_shape, region, {0, 0, 0}, nullptr);
            outline_shape = block.getOutline(region, {0, 0, 0}, outline_shape);
            visual_shape = block.getVisualShape(visual_shape);
            ui_shape = block.getUIShape(ui_shape);
            block.getLiquidClipShape(region, {0, 0, 0}, liquid_clip_shape);
            auto map_color = block.getLegacyBlock().getMapColor(region, {0, 10, 0}, block);
            data.block_states.push_back({
                {"name", name},
                {"blockStateHash", block.getRuntimeId()},
                {"burnOdds", block.getBurnOdds()},
                {"flameOdds", block.getFlameOdds()},
                {"thickness", round(block.getThickness())},
                {"lightDampening", block.getLight()},
                {"lightEmission", block.getLightEmission()},
                {"explosionResistance", round(block.getExplosionResistance())},
                {"friction", round(block.getFriction())},
                {"hardness", round(block.getDestroySpeed())},
                {"canContainLiquid", block.canContainLiquid()},
                {"requiresCorrectToolForDrops", block.requiresCorrectToolForDrops()},
                {"isSolid", block.isSolid()},
                {"translucency", block.getTranslucency()},
                {"mapColor", map_color.toHexString()},
                {"collisionShape",
                 {
                     round(collision_shape.min.x),
                     round(collision_shape.min.y),
                     round(collision_shape.min.z),
                     round(collision_shape.max.x),
                     round(collision_shape.max.y),
                     round(collision_shape.max.z),
                 }},
                {"outlineShape",
                 {
                     round(outline_shape.min.x),
                     round(outline_shape.min.y),
                     round(outline_shape.min.z),
                     round(outline_shape.max.x),
                     round(outline_shape.max.y),
                     round(outline_shape.max.z),
                 }},
                {"visualShape",
                 {
                     round(visual_shape.min.x),
                     round(visual_shape.min.y),
                     round(visual_shape.min.z),
                     round(visual_shape.max.x),
                     round(visual_shape.max.y),
                     round(visual_shape.max.z),
                 }},
                {"uiShape",
                 {
                     round(ui_shape.min.x),
                     round(ui_shape.min.y),
                     round(ui_shape.min.z),
                     round(ui_shape.max.x),
                     round(ui_shape.max.y),
                     round(ui_shape.max.z),
                 }},
                {"liquidClipShape",
                 {
                     round(liquid_clip_shape.min.x),
                     round(liquid_clip_shape.min.y),
                     round(liquid_clip_shape.min.z),
                     round(liquid_clip_shape.max.x),
                     round(liquid_clip_shape.max.y),
                     round(liquid_clip_shape.max.z),
                 }},
            });
            data.block_palette.add(block.getSerializationId().copy());
            return true;
        });
        return true;
    });
}

void dumpItemData(VanillaData &data, ::Level &level)
{
    auto item_registry = level.getItemRegistry();
    for (const auto &[key, item] : item_registry.getNameToItemMap()) {
        const auto &name = item->getFullItemName();
        nlohmann::json tags;
        for (const auto &tag : item->getTags()) {
            auto tag_name = tag.getString();
            tags.push_back(tag_name);

            if (!data.item_tags.contains(tag_name)) {
                data.item_tags[tag_name] = {};
            }
            data.item_tags[tag_name].push_back(name);
        }

        data.items[name] = {
            {"id", item->getId()},
            {"attackDamage", item->getAttackDamage()},
            {"armorValue", item->getArmorValue()},
            {"enchantValue", item->getEnchantValue()},
            {"toughnessValue", item->getToughnessValue()},
            {"maxDamage", item->getMaxDamage()},
            {"isDamageable", item->isDamageable()},
            {"maxStackSize", item->getMaxStackSize({})},
            {"furnaceBurnDuration", FurnaceBlockActor::getBurnDuration(*::ItemStack::create(*item), 200)},
            {"furnaceXPMultiplier", item->getFurnaceXPmultiplier(nullptr)}};
        if (!tags.is_null()) {
            data.items[name]["tags"] = tags;
        }
    }

    auto creative_item_registry = item_registry.getCreativeItemRegistry();
    creative_item_registry->forEachCreativeItemInstance([&](const ItemInstance &item_instance) {
        CompoundTag tag;
        tag.putString("name", item_instance.getItem()->getFullItemName());
        tag.putShort("damage", static_cast<std::int16_t>(item_instance.getAuxValue()));

        if (const auto *user_data = item_instance.getUserData(); user_data) {
            tag.putCompound("tag", user_data->clone());
        }

        if (item_instance.isBlock()) {
            tag.putInt("blockStateHash", static_cast<std::int32_t>(item_instance.getBlock()->getRuntimeId()));
        }
        data.creative_items.add(tag.copy());
        return true;
    });
}

void dumpShapedRecipe(const Recipe &recipe, nlohmann::json &json)
{
    auto input = json["input"];
    json.erase("input");
    char next_key = 'A';
    std::unordered_map<std::string, char> ingredient_key;
    for (int i = 0; i < recipe.getHeight(); i++) {
        std::string pattern;
        for (int j = 0; j < recipe.getWidth(); j++) {
            const auto &ingredient = input[j + i * recipe.getWidth()];
            if (ingredient["count"] == 0) {
                pattern.push_back(' ');
                continue;
            }
            auto name_or_tag = ingredient.contains("tag") ? ingredient["tag"] : ingredient["item"];
            if (ingredient_key.find(name_or_tag) == ingredient_key.end()) {
                json["input"][std::string{next_key}] = ingredient;
                ingredient_key[name_or_tag] = next_key++;
            }
            pattern.push_back(ingredient_key[name_or_tag]);
        }
        json["pattern"].push_back(pattern);
    }
    json["width"] = recipe.getWidth();
    json["height"] = recipe.getHeight();
}

void dumpRecipes(VanillaData &data, ::Level &level)
{
    auto packet = CraftingDataPacket::prepareFromRecipes(level.getRecipes(), false);
    auto id_to_name = [&level](int id) {
        return level.getItemRegistry().getItem(id)->getFullItemName();
    };

    for (const auto &entry : packet->crafting_entries) {
        nlohmann::json recipe;
        if (entry.recipe) {
            recipe["id"] = entry.recipe->getRecipeId();
            recipe["netId"] = entry.recipe->getNetId().raw_id;
            recipe["uuid"] = EndstoneUUID::fromMinecraft(entry.recipe->getId()).str();
            recipe["tag"] = entry.recipe->getTag().getString();
            recipe["priority"] = entry.recipe->getPriority();

            for (const auto &ingredient : entry.recipe->getIngredients()) {
                recipe["input"].push_back({{"count", ingredient.getStackSize()}});
                if (ingredient.impl && ingredient.impl->getType() == ItemDescriptor::InternalType::ItemTag) {
                    recipe["input"].back()["tag"] =
                        static_cast<ItemDescriptor::ItemTagDescriptor *>(ingredient.impl.get())->item_tag.getString();
                }
                else {
                    recipe["input"].back()["item"] = ingredient.getFullName();
                }
                if (ingredient.getAuxValue() != 0 && ingredient.getAuxValue() != 0x7fff) {
                    recipe["input"].back()["data"] = ingredient.getAuxValue();
                }
            }

            for (const auto &result_item : entry.recipe->getResultItems()) {
                recipe["output"].push_back({
                    {"item", result_item.getItem()->getFullItemName()},
                    {"count", result_item.getCount()},
                });
                if (result_item.getAuxValue() != 0 && result_item.getAuxValue() != 0x7fff) {
                    recipe["output"].back()["data"] = result_item.getAuxValue();
                }
                if (result_item.hasUserData()) {
                    BigEndianStringByteOutput output;
                    NbtIo::writeNamedTag("", *result_item.getUserData(), output);
                    recipe["output"].back()["nbt"] = base64_encode(output.buffer);
                }
            }
        }
        else {
            recipe["tag"] = entry.tag.getString();
            recipe["input"] = {
                {"item", id_to_name(entry.item_data)},
            };
            if (entry.item_aux != 0 && entry.item_aux != 0x7fff) {
                recipe["input"]["data"] = entry.item_aux;
            }
            recipe["output"] = {
                {"item", entry.item_result.getFullName()},
                {"count", entry.item_result.getStackSize()},
            };
            if (entry.item_result.getAuxValue() != 0 && entry.item_result.getAuxValue() != 0x7fff) {
                recipe["output"]["data"] = entry.item_result.getAuxValue();
            }
        }

        switch (entry.entry_type) {
        case CraftingDataEntryType::ShapelessRecipe: {
            data.recipes.shapeless.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::ShapedRecipe: {
            dumpShapedRecipe(*entry.recipe, recipe);
            data.recipes.shaped.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::FurnaceRecipe: {
            data.recipes.furnace.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::FurnaceAuxRecipe: {
            data.recipes.furnace_aux.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::MultiRecipe: {
            data.recipes.multi.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::UserDataShapelessRecipe: {
            data.recipes.user_data_shapeless.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::ShapelessChemistryRecipe: {
            data.recipes.shapeless_chemistry.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::ShapedChemistryRecipe: {
            dumpShapedRecipe(*entry.recipe, recipe);
            data.recipes.shaped_chemistry.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::SmithingTransformRecipe: {
            recipe["template"] = recipe["input"][0];
            recipe["base"] = recipe["input"][1];
            recipe["addition"] = recipe["input"][2];
            recipe.erase("input");
            data.recipes.smithing_transform.push_back(recipe);
            break;
        }
        case CraftingDataEntryType::SmithingTrimRecipe: {
            recipe["template"] = recipe["input"][0];
            recipe["base"] = recipe["input"][1];
            recipe["addition"] = recipe["input"][2];
            recipe.erase("input");
            data.recipes.smithing_trim.push_back(recipe);
            break;
        }
        default:
            throw std::runtime_error("Unknown craft data type");
        }
    }

    for (const auto &entry : packet->potion_mix_entries) {
        data.recipes.potion_mixes.push_back({
            {"input", {{"item", id_to_name(entry.from_item_id)}, {"data", entry.from_item_aux}}},
            {"reagent", {{"item", id_to_name(entry.reagent_item_id)}, {"data", entry.reagent_item_aux}}},
            {"output", {{"item", id_to_name(entry.to_item_id)}, {"data", entry.to_item_aux}}},
        });
    }

    for (const auto &entry : packet->container_mix_entries) {
        data.recipes.container_mixes.push_back({
            {"input", id_to_name(entry.from_item_id)},
            {"reagent", id_to_name(entry.reagent_item_id)},
            {"output", id_to_name(entry.to_item_id)},
        });
    }

    for (const auto &entry : packet->material_reducer_entries) {
        nlohmann::json json = {
            {"input", entry.from_item_key},
            {"outputs", {}},
        };
        for (const auto &item : entry.to_item_ids_and_counts) {
            json["outputs"].push_back({
                {"item", id_to_name(item.to_item_id)},
                {"count", item.to_item_count},
            });
        }
        data.recipes.material_reducer.push_back(json);
    }
}

void dumpBiomes(VanillaData &data, ::Level &level)
{
    auto &biomes = data.biomes;
    level.getBiomeRegistry().forEachBiome([&biomes](const Biome &biome) {
        biomes[biome.getName()] = {{"id", biome.getId()}};
    });
}

}  // namespace

VanillaData *VanillaData::get()
{
    static std::atomic<bool> ready = false;
    static std::atomic<bool> should_run = true;

    if (ready) {
        return &entt::locator<VanillaData>::value();
    }

    if (entt::locator<EndstoneServer>::has_value()) {
        auto &server = entt::locator<EndstoneServer>::value();
        if (auto *server_level = server.getLevel(); server_level) {
            auto &level = static_cast<EndstoneLevel *>(server_level)->getHandle();
            auto &scheduler = static_cast<EndstoneScheduler &>(server.getScheduler());
            if (should_run) {
                scheduler.runTask([&]() {
                    // run on the server thread instead of UI thread
                    VanillaData data;
                    dumpBlockData(data, level);
                    dumpItemData(data, level);
                    dumpRecipes(data, level);
                    dumpBiomes(data, level);
                    entt::locator<VanillaData>::emplace(std::move(data));
                    ready = true;
                });
                should_run = false;
            }
        }
    }
    return nullptr;
}

}  // namespace endstone::detail::devtools
