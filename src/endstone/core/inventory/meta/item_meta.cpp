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

#include "endstone/inventory/meta/item_meta.h"

#include <utility>

#include "bedrock/world/item/enchanting/enchant.h"
#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_metas.h"

namespace endstone::core {
namespace {
std::unique_ptr<ListTag> createStringList(const std::vector<std::string> &list)
{
    auto list_tag = std::make_unique<ListTag>();
    for (const auto &value : list) {
        list_tag->add(std::make_unique<StringTag>(value));
    }
    return list_tag;
}

void setDisplayTag(::CompoundTag &tag, std::string key, std::unique_ptr<Tag> value)
{
    auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY);
    if (!display) {
        display = tag.putCompound(ItemStackBase::TAG_DISPLAY, std::make_unique<CompoundTag>());
    }
    display->put(std::move(key), std::move(value));
}

std::unordered_map<std::string, int> buildEnchantments(const ::CompoundTag &tag)
{
    if (!tag.contains(ItemStackBase::TAG_ENCHANTS)) {
        return {};
    }

    const auto *ench = tag.getList(ItemStackBase::TAG_ENCHANTS);
    if (!ench) {
        return {};
    }

    std::unordered_map<std::string, int> enchantments;
    ench->forEachCompoundTag([&](const CompoundTag &enchant_tag) {
        auto id = enchant_tag.getShort("id");
        auto lvl = enchant_tag.getShort("lvl") & 0xffff;
        const auto *enchant = Enchant::getEnchant(static_cast<Enchant::Type>(id));
        if (enchant) {
            enchantments[enchant->getStringId().getString()] = lvl;
        }
    });
    return enchantments;
}

void applyEnchantments(const std::unordered_map<std::string, int> &enchantments, ::CompoundTag &tag)
{
    tag.remove(ItemStackBase::TAG_ENCHANTS);
    if (enchantments.empty()) {
        return;
    }

    auto list = std::make_unique<ListTag>();
    for (const auto &[key, lvl] : enchantments) {
        auto subtag = std::make_unique<CompoundTag>();
        const auto *enchant = Enchant::getEnchantFromName(key);
        if (!enchant) {
            continue;
        }
        subtag->putShort("id", static_cast<std::int16_t>(enchant->getEnchantType()));
        subtag->putShort("lvl", static_cast<std::int16_t>(lvl));
        list->add(std::move(subtag));
    }
    tag.put(ItemStackBase::TAG_ENCHANTS, std::move(list));
}
}  // namespace

template <>
bool EndstoneItemMetas::applicableTo<ItemMeta>(const std::string &type)
{
    return type != "minecraft:air";
}

template <>
void EndstoneItemMetas::applyToItem<ItemMeta>(const ItemMeta &self, ::CompoundTag &tag)
{
    if (auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
        display->remove(ItemStackBase::TAG_DISPLAY_NAME);
        display->remove(ItemStackBase::TAG_LORE);
    }
    if (self.hasDisplayName()) {
        setDisplayTag(tag, ItemStackBase::TAG_DISPLAY_NAME, std::make_unique<StringTag>(self.getDisplayName()));
    }
    if (self.hasLore()) {
        setDisplayTag(tag, ItemStackBase::TAG_LORE, createStringList(self.getLore()));
    }

    applyEnchantments(self.getEnchants(), tag);

    tag.remove(ItemStackBase::TAG_REPAIR_COST);
    if (self.hasRepairCost()) {
        tag.putInt(ItemStackBase::TAG_REPAIR_COST, self.getRepairCost());
    }

    tag.remove("Unbreakable");
    if (self.isUnbreakable()) {
        tag.putBoolean("Unbreakable", self.isUnbreakable());
    }

    tag.remove(::Item::TAG_DAMAGE);
    if (self.hasDamage()) {
        tag.putInt(::Item::TAG_DAMAGE, self.getDamage());
    }
}

template <>
bool EndstoneItemMetas::equalsCommon<ItemMeta>(const ItemMeta &self, const ItemMeta &that)
{
    return (self.hasDisplayName() ? that.hasDisplayName() && self.getDisplayName() == that.getDisplayName()
                                  : !that.hasDisplayName())  //
        && (self.hasEnchants() ? that.hasEnchants() && self.getEnchants() == that.getEnchants()
                               : !that.hasEnchants())  //
        && (self.getLore() == that.getLore())          //
        && (self.hasRepairCost() ? that.hasRepairCost() && self.getRepairCost() == that.getRepairCost()
                                 : !that.hasRepairCost())  //
        && (self.isUnbreakable() == that.isUnbreakable())  //
        && (self.hasDamage() ? that.hasDamage() && self.getDamage() == that.getDamage() : !that.hasDamage());
}

template <>
bool EndstoneItemMetas::notUncommon<ItemMeta>(const ItemMeta &, const ItemMeta &)
{
    return true;
}

template <>
void EndstoneItemMetas::loadFrom<ItemMeta>(ItemMeta &self, const ::ItemStackBase &item)
{
    if (!item.hasUserData()) {
        return;
    }

    auto &tag = *item.getUserData();
    if (const auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
        auto display_name = display->getString(ItemStackBase::TAG_DISPLAY_NAME);
        self.setDisplayName(display_name);

        if (const auto *lore_tag = display->getList(ItemStackBase::TAG_LORE)) {
            std::vector<std::string> lore;
            lore.reserve(lore_tag->size());
            for (auto i = 0; i < lore_tag->size(); i++) {
                lore.emplace_back(lore_tag->getString(i));
            }
            self.setLore(lore);
        }
    }

    for (auto &[id, lvl] : buildEnchantments(tag)) {
        self.addEnchant(id, lvl, true);
    }
    self.setRepairCost(tag.getInt(ItemStackBase::TAG_REPAIR_COST));
    self.setUnbreakable(tag.getBoolean("Unbreakable"));
    self.setDamage(tag.getInt(::Item::TAG_DAMAGE));
}
}  // namespace endstone::core
