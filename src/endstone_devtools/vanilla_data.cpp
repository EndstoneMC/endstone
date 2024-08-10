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

#include <entt/entt.hpp>
#include <magic_enum/magic_enum.hpp>

#include "bedrock/common/util/bytes_data_output.h"
#include "bedrock/nbt/nbt_io.h"
#include "bedrock/network/packet/crafting_data_packet.h"
#include "bedrock/world/item/registry/creative_item_registry.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/detail/base64.h"
#include "endstone/detail/devtools/imgui/imgui_json.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"

namespace endstone::detail::devtools {

nlohmann::json toJson(const Tag &tag)  // NOLINT(*-no-recursion)
{
    switch (tag.getId()) {
    case Tag::Type::Byte: {
        const auto &t = static_cast<const ByteTag &>(tag);
        return t.data;
    }
    case Tag::Type::Short: {
        const auto &t = static_cast<const ShortTag &>(tag);
        return t.data;
    }
    case Tag::Type::Int: {
        const auto &t = static_cast<const IntTag &>(tag);
        return t.data;
    }
    case Tag::Type::Int64: {
        const auto &t = static_cast<const Int64Tag &>(tag);
        return t.data;
    }
    case Tag::Type::Float: {
        const auto &t = static_cast<const FloatTag &>(tag);
        return t.data;
    }
    case Tag::Type::Double: {
        const auto &t = static_cast<const DoubleTag &>(tag);
        return t.data;
    }
    case Tag::Type::String: {
        const auto &t = static_cast<const StringTag &>(tag);
        return t.data;
    }
    case Tag::Type::List: {
        nlohmann::json array;
        const auto &t = static_cast<const ListTag &>(tag);
        for (auto i = 0; i < t.size(); i++) {
            array.push_back(toJson(*t.get(i)));
        }
        return array;
    }
    case Tag::Type::Compound: {
        nlohmann::json object;
        const auto &t = static_cast<const CompoundTag &>(tag);
        for (const auto &[key, value] : t) {
            object[key] = toJson(*value.get());
        }
        return object;
    }
    case Tag::Type::IntArray: {
        nlohmann::json array;
        const auto &t = static_cast<const IntArrayTag &>(tag);
        for (const auto &i : t.data) {
            array.push_back(i);
        }
        return array;
    }
    case Tag::Type::ByteArray: {
        nlohmann::json array;
        const auto &t = static_cast<const ByteArrayTag &>(tag);
        for (const auto &i : t.data) {
            array.push_back(i);
        }
        return array;
    }
    case Tag::Type::End:
    default:
        return {};
    }
}

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
    auto item_registry = level.getItemRegistry().weak_registry.lock();

    BlockTypeRegistry::forEachBlock([&](const BlockLegacy &block_legacy) {
        const auto &material = block_legacy.getMaterial();
        auto material_name = magic_enum::enum_name(material.getType());
        data.materials[material_name] = {
            {"isNeverBuildable", material.isNeverBuildable()},
            {"isAlwaysDestroyable", material.isAlwaysDestroyable()},
            {"isReplaceable", material.isReplaceable()},
            {"isLiquid", material.isLiquid()},
            {"translucency", round(material.getTranslucency())},
            {"blocksMotion", material.getBlocksMotion()},
            {"blocksPrecipitation", material.getBlocksPrecipitation()},
            {"isSolid", material.isSolid()},
            {"isSuperHot", material.isSuperHot()},
            {"isSolidBlocking", material.isSolidBlocking()},
        };

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

        data.block_types[name] = {{"material", magic_enum::enum_name(material.getType())}};
        if (!tags.is_null()) {
            data.block_types[name]["tags"] = tags;
        }

        nlohmann::json special_tools;
        for (const auto &[key, item] : item_registry->getNameToItemMap()) {
            if (item->canDestroySpecial(*block_legacy.getDefaultState())) {
                special_tools.push_back(item->getFullItemName());
            }
        }
        if (!special_tools.is_null()) {
            data.block_types[name]["specialTools"] = special_tools;
        }

        block_legacy.forEachBlockPermutation([&](const ::Block &block) {
            AABB collision_shape = {0};
            block.getCollisionShape(collision_shape, region, {0, 0, 0}, nullptr);
            auto map_color = block.getLegacyBlock().getMapColor(region, {0, 10, 0}, block);
            data.block_states.push_back({
                {"name", name},
                {"blockStateHash", block.getRuntimeId()},
                {"burnOdds", block.getBurnOdds()},
                {"flameOdds", block.getFlameOdds()},
                {"thickness", round(block.getThickness())},
                {"light", block.getLight()},
                {"lightEmission", block.getLightEmission()},
                {"explosionResistance", round(block.getExplosionResistance())},
                {"friction", round(block.getFriction())},
                {"hardness", round(block.getDestroySpeed())},
                {"canContainLiquid", block.getLegacyBlock().canContainLiquid()},
                // {"canDropWithAnyTool", block.canDropWithAnyTool()},
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
            });
            data.block_palette.add(block.getSerializationId().copy());
            return true;
        });
        return true;
    });
}

void dumpItemData(VanillaData &data, ::Level &level)
{
    auto item_registry = level.getItemRegistry().weak_registry.lock();
    for (const auto &[key, item] : item_registry->getNameToItemMap()) {
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
            {"toughnessValue", item->getToughnessValue()},
            {"maxDamage", item->getMaxDamage()},
            {"isDamageable", item->isDamageable()},
            {"maxStackSize", item->getMaxStackSize({})},
            {"furnaceBurnIntervalMultipler", item->getFurnaceBurnIntervalMultipler()},
            {"furnaceXPMultiplier", item->getFurnaceXPmultiplier(nullptr)}
        };
        if (!tags.is_null()) {
            data.items[name]["tags"] = tags;
        }
    }

    CreativeItemRegistry::forEachCreativeItemInstance([&](ItemInstance &item_instance) {
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
        return level.getItemRegistry().weak_registry.lock()->getItem(id)->getFullItemName();
    };

    for (const auto &entry : packet->crafting_entries) {
        nlohmann::json recipe;
        if (entry.recipe) {
            recipe["id"] = entry.recipe->getRecipeId();
            recipe["netId"] = entry.recipe->getNetId().raw_id;
            recipe["uuid"] = entry.recipe->getId().toEndstone().str();
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
        case ShapelessRecipe: {
            data.recipes.shapeless.push_back(recipe);
            break;
        }
        case ShapedRecipe: {
            dumpShapedRecipe(*entry.recipe, recipe);
            data.recipes.shaped.push_back(recipe);
            break;
        }
        case FurnaceRecipe: {
            data.recipes.furnace.push_back(recipe);
            break;
        }
        case FurnaceAuxRecipe: {
            data.recipes.furnace_aux.push_back(recipe);
            break;
        }
        case MultiRecipe: {
            data.recipes.multi.push_back(recipe);
            break;
        }
        case ShulkerBoxRecipe: {
            data.recipes.shulker_box.push_back(recipe);
            break;
        }
        case ShapelessChemistryRecipe: {
            data.recipes.shapeless_chemistry.push_back(recipe);
            break;
        }
        case ShapedChemistryRecipe: {
            dumpShapedRecipe(*entry.recipe, recipe);
            data.recipes.shaped_chemistry.push_back(recipe);
            break;
        }
        case SmithingTransformRecipe: {
            recipe["template"] = recipe["input"][0];
            recipe["base"] = recipe["input"][1];
            recipe["addition"] = recipe["input"][2];
            recipe.erase("input");
            data.recipes.smithing_transform.push_back(recipe);
            break;
        }
        case SmithingTrimRecipe: {
            recipe["template"] = recipe["input"][0];
            recipe["base"] = recipe["input"][1];
            recipe["addition"] = recipe["input"][2];
            recipe.erase("input");
            data.recipes.smithing_trim.push_back(recipe);
            break;
        }
        default:
        case COUNT:
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
