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

#include "endstone/core/devtools/vanilla_data.h"

#include <unordered_map>

#include <entt/locator/locator.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/nbt/nbt_io.h"
#include "bedrock/network/packet/crafting_data_packet.h"
#include "bedrock/util/string_byte_output.h"
#include "bedrock/world/item/registry/creative_item_registry.h"
#include "bedrock/world/level/block/actor/furnace_block_actor.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/core/base64.h"
#include "endstone/core/devtools/imgui_json.h"
#include "endstone/core/json.h"
#include "endstone/core/level/level.h"
#include "endstone/core/server.h"
#include "endstone/core/util/uuid.h"

inline double truncate(const double value, int decimal_places = 6)
{
    const double scale = std::pow(10.0, decimal_places);
    return std::trunc(value * scale) / scale;
}

inline void to_json(nlohmann::json &json, const AABB &aabb)
{
    json.push_back(truncate(aabb.min.x));
    json.push_back(truncate(aabb.min.y));
    json.push_back(truncate(aabb.min.z));
    json.push_back(truncate(aabb.max.x));
    json.push_back(truncate(aabb.max.y));
    json.push_back(truncate(aabb.max.z));
}

namespace endstone::core::devtools {
namespace {
void dumpBlockData(VanillaData &data, const ::Level &level)
{
    auto overworld = level.getDimension(VanillaDimensions::Overworld);
    auto &region = overworld.unwrap()->getBlockSourceFromMainChunkSource();
    auto item_registry = level.getItemRegistry();
    auto block_registry = level.getBlockTypeRegistry();
    block_registry->forEachBlockType([&](const BlockType &block_type) {
        const auto &name = block_type.getName().getString();

        nlohmann::json tags;
        for (const auto &tag : block_type.getTags()) {
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

        data.block_types[name] = {{"defaultBlockStateHash", block_type.getDefaultState().getRuntimeId()}};
        if (!tags.is_null()) {
            data.block_types[name]["tags"] = tags;
        }

        block_type.forEachBlockPermutation([&](const ::Block &block) {
            std::vector<AABB> collision_shape;
            AABB outline_shape;
            AABB visual_shape;
            AABB ui_shape;
            AABB liquid_clip_shape;
            block.addCollisionShapes(region, {0, 0, 0}, nullptr, collision_shape, nullptr);
            outline_shape = block.getOutline(region, {0, 0, 0}, outline_shape);
            visual_shape = block.getVisualShape(visual_shape);
            ui_shape = block.getUIShape(ui_shape);
            block.getLiquidClipVolume(region, {0, 0, 0}, liquid_clip_shape);
            auto map_color = block.getBlockType().getMapColor(region, {0, 10, 0}, block);
            data.block_states.push_back({
                {"name", name},
                {"blockStateHash", block.getRuntimeId()},
                {"burnOdds", block.getBurnOdds()},
                {"flameOdds", block.getFlameOdds()},
                {"thickness", truncate(block.getThickness())},
                {"lightDampening", block.getLight()},
                {"lightEmission", block.getLightEmission()},
                {"explosionResistance", truncate(block.getExplosionResistance())},
                {"friction", truncate(block.getFriction())},
                {"hardness", truncate(block.getDestroySpeed())},
                {"canContainLiquidSource", block.getDirectData().water_detection_rule.can_contain_liquid},
                {"liquidReactionOnTouch",
                 magic_enum::enum_name(block.getDirectData().water_detection_rule.on_liquid_touches)},
                {"requiresCorrectToolForDrops", block.requiresCorrectToolForDrops()},
                {"isSolid", block.isSolid()},
                {"translucency", block.getTranslucency()},
                {"mapColor", map_color.toHexString()},
                {"tintMethod", magic_enum::enum_name(block.getBlockType().getTintMethod())},
                {"collisionShape", collision_shape},
                {"outlineShape", outline_shape},
                {"visualShape", visual_shape},
                {"uiShape", ui_shape},
                {"liquidClipShape", liquid_clip_shape},
            });
            data.block_palette.add(block.getSerializationId().copy());
            return true;
        });
        return true;
    });
}

void dumpItemData(VanillaData &data, const ::Level &level)
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

        data.items[name] = {{"id", item->getId()},
                            {"attackDamage", item->getAttackDamage()},
                            {"armorValue", item->getArmorValue()},
                            {"enchantValue", item->getEnchantValue()},
                            {"toughnessValue", item->getToughnessValue()},
                            {"maxDamage", item->getMaxDamage()},
                            {"isDamageable", item->isDamageable()},
                            {"maxStackSize", item->getMaxStackSize(ItemDescriptor())},
                            {"furnaceBurnDuration", FurnaceBlockActor::getBurnDuration(::ItemStack(*item), 200)},
                            {"furnaceXPMultiplier", item->getFurnaceXPmultiplier(nullptr)}};

        if (const auto components = item->buildNetworkTag()) {
            ::CompoundTag tag;
            tag.putCompound("components", components->clone());
            tag.putBoolean("isComponentBased", item->isComponentBased());
            tag.putInt("version", static_cast<std::int32_t>(item->item_parse_version));
            data.item_components.put(name, std::move(tag));
        }

        if (!tags.is_null()) {
            data.items[name]["tags"] = tags;
        }
    }

    auto creative_item_registry = item_registry.getCreativeItemRegistry();
    for (const auto &creative_item_entry : creative_item_registry->getCreativeItemEntries()) {
        const ItemInstance &item_instance = creative_item_entry.getItemInstance();

        ::CompoundTag tag;
        tag.putString("name", item_instance.getItem()->getFullItemName());
        tag.putInt64("groupIndex", creative_item_entry.getGroup()->getIndex());
        tag.putShort("damage", static_cast<std::int16_t>(item_instance.getAuxValue()));
        tag.putString("category", std::string(magic_enum::enum_name(item_instance.getItem()->getCreativeCategory())));

        if (const auto *user_data = item_instance.getUserData(); user_data) {
            tag.putCompound("tag", user_data->clone());
        }

        data.creative_items.add(tag.copy());
    }

    for (const auto &creative_group : creative_item_registry->getCreativeGroups()) {
        data.creative_groups.push_back({
            {"name", creative_group.getName()},
            {"category", magic_enum::enum_name(creative_group.getCreativeCategory())},
        });
        if (auto *icon_item = creative_group.getIcon().getItem(); icon_item) {
            data.creative_groups.back()["icon"] = icon_item->getFullItemName();
        }
    }
}

void dumpShapedRecipe(int width, int height, nlohmann::json &json)
{
    auto input = json["input"];
    json.erase("input");
    char next_key = 'A';
    std::unordered_map<std::string, char> ingredient_key;
    for (int i = 0; i < height; i++) {
        std::string pattern;
        for (int j = 0; j < width; j++) {
            const auto &ingredient = input[j + i * width];
            if (ingredient["count"] == 0) {
                pattern.push_back(' ');
                continue;
            }
            auto name_or_tag = ingredient.contains("tag") ? ingredient["tag"] : ingredient["item"];
            if (!ingredient_key.contains(name_or_tag)) {
                json["input"][std::string{next_key}] = ingredient;
                ingredient_key[name_or_tag] = next_key++;
            }
            pattern.push_back(ingredient_key[name_or_tag]);
        }
        json["pattern"].push_back(pattern);
    }
    json["width"] = width;
    json["height"] = height;
}

void dumpRecipes(VanillaData &data, ::Level &level)
{
    auto payload = CraftingDataPacket::prepareFromRecipes(level.getRecipes(), false);
    auto id_to_name = [&level](int id) {
        return level.getItemRegistry().getItem(id)->getFullItemName();
    };

    auto dump_ingredient = [](const RecipeIngredient &ingredient, nlohmann::json &json) {
        json.push_back({{"count", ingredient.getStackSize()}});
        Json::Value json_value;
        ingredient.serialize(json_value);
        if (nlohmann::json value = json_value; value.is_object()) {
            json.back()["tag"] = value.at("item_tag").get<std::string>();
        }
        else {
            json.back()["item"] = ingredient.getFullName();
        }
        if (ingredient.getAuxValue() != 0 && ingredient.getAuxValue() != ItemDescriptor::ANY_AUX_VALUE) {
            json.back()["data"] = ingredient.getAuxValue();
        }
    };

    // The wire form carries only a numeric item id, so the name is resolved through the registry.
    auto dump_result = [&id_to_name](const NetworkItemInstanceDescriptorData &result, nlohmann::json &json) {
        json.push_back({
            {"item", id_to_name(result.id)},
            {"count", result.stack_size},
        });
        if (result.aux_value != 0 && result.aux_value != ItemDescriptor::ANY_AUX_VALUE) {
            json.back()["data"] = result.aux_value;
        }
        if (!result.user_data_buffer.empty()) {
            json.back()["nbt"] = core::base64_encode(result.user_data_buffer);
        }
    };

    auto dump_shapeless = [&](const ShapelessRecipePayload &entry) {
        nlohmann::json recipe;
        recipe["id"] = entry.recipe_id;
        recipe["netId"] = entry.net_id.raw_id;
        recipe["uuid"] = core::EndstoneUUID::fromMinecraft(entry.uuid).str();
        recipe["tag"] = entry.tag;
        recipe["priority"] = entry.priority;
        for (const auto &ingredient : entry.ingredients) {
            dump_ingredient(ingredient, recipe["input"]);
        }
        for (const auto &result : entry.results) {
            dump_result(result, recipe["output"]);
        }
        return recipe;
    };

    auto dump_shaped = [&](const ShapedRecipePayload &entry) {
        nlohmann::json recipe;
        recipe["id"] = entry.recipe_id;
        recipe["netId"] = entry.net_id.raw_id;
        recipe["uuid"] = core::EndstoneUUID::fromMinecraft(entry.uuid).str();
        recipe["tag"] = entry.tag;
        recipe["priority"] = entry.priority;
        for (const auto &ingredient : entry.ingredients) {
            dump_ingredient(ingredient, recipe["input"]);
        }
        for (const auto &result : entry.results) {
            dump_result(result, recipe["output"]);
        }
        dumpShapedRecipe(entry.width, entry.height, recipe);
        return recipe;
    };

    // Smithing recipes carry three fixed ingredient slots and no uuid or priority.
    auto dump_smithing = [&](const std::string &recipe_id, RecipeNetId net_id, const std::string &tag,
                             std::initializer_list<const RecipeIngredient *> ingredients) {
        nlohmann::json recipe;
        recipe["id"] = recipe_id;
        recipe["netId"] = net_id.raw_id;
        recipe["tag"] = tag;
        nlohmann::json input;
        for (const auto *ingredient : ingredients) {
            dump_ingredient(*ingredient, input);
        }
        recipe["template"] = input[0];
        recipe["base"] = input[1];
        recipe["addition"] = input[2];
        return recipe;
    };

    for (const auto &entry : payload.shapeless_recipes) {
        data.recipes.shapeless.push_back(dump_shapeless(entry));
    }
    for (const auto &entry : payload.user_data_shapeless_recipes) {
        data.recipes.user_data_shapeless.push_back(dump_shapeless(entry));
    }
    for (const auto &entry : payload.shapeless_chemistry_recipes) {
        data.recipes.shapeless_chemistry.push_back(dump_shapeless(entry));
    }
    for (const auto &entry : payload.shaped_recipes) {
        data.recipes.shaped.push_back(dump_shaped(entry));
    }
    for (const auto &entry : payload.shaped_chemistry_recipes) {
        data.recipes.shaped_chemistry.push_back(dump_shaped(entry));
    }
    for (const auto &entry : payload.multi_recipes) {
        data.recipes.multi.push_back({
            {"netId", entry.net_id.raw_id},
            {"uuid", core::EndstoneUUID::fromMinecraft(entry.uuid).str()},
        });
    }
    for (const auto &entry : payload.smithing_transform_recipes) {
        auto recipe = dump_smithing(entry.recipe_id, entry.net_id, entry.tag,
                                    {&entry.template_ingredient, &entry.base_ingredient, &entry.addition_ingredient});
        dump_result(entry.result, recipe["output"]);
        data.recipes.smithing_transform.push_back(recipe);
    }
    for (const auto &entry : payload.smithing_trim_recipes) {
        data.recipes.smithing_trim.push_back(
            dump_smithing(entry.recipe_id, entry.net_id, entry.tag,
                          {&entry.template_ingredient, &entry.base_ingredient, &entry.addition_ingredient}));
    }

    for (const auto &entry : payload.potion_mixes) {
        data.recipes.potion_mixes.push_back({
            {"input", {{"item", id_to_name(entry.from_item_id)}, {"data", entry.from_item_aux}}},
            {"reagent", {{"item", id_to_name(entry.reagent_item_id)}, {"data", entry.reagent_item_aux}}},
            {"output", {{"item", id_to_name(entry.to_item_id)}, {"data", entry.to_item_aux}}},
        });
    }

    for (const auto &entry : payload.container_mixes) {
        data.recipes.container_mixes.push_back({
            {"input", id_to_name(entry.from_item_id)},
            {"reagent", id_to_name(entry.reagent_item_id)},
            {"output", id_to_name(entry.to_item_id)},
        });
    }

    for (const auto &entry : payload.material_reducers) {
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
    level.getBiomeRegistry().forEachBiome(
        [&biomes](const Biome &biome) { biomes[biome.getFullName()] = {{"id", biome.getId()}}; });
}

}  // namespace

VanillaData *VanillaData::get()
{
    static std::atomic ready = false;
    static std::atomic should_run = true;

    if (ready) {
        return &entt::locator<VanillaData>::value();
    }

    if (entt::locator<EndstoneServer>::has_value()) {
        auto &server = EndstoneServer::getInstance();
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

}  // namespace endstone::core::devtools
