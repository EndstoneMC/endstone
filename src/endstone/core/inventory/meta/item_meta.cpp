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

#include "endstone/core/inventory/meta/item_meta.h"

#include <utility>

#include <fmt/chrono.h>

#include "bedrock/world/item/enchanting/enchant.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack_base.h"

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

EndstoneItemMeta::EndstoneItemMeta(const EndstoneItemMeta *meta)
{
    if (meta == nullptr) {
        return;
    }
    *this = *meta;
}

EndstoneItemMeta::EndstoneItemMeta(const ::CompoundTag &tag)
{
    if (const auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
        // Display name
        auto display_name = display->getString(ItemStackBase::TAG_DISPLAY_NAME);
        if (!display_name.empty()) {
            display_name_ = display_name;
        }

        // Lore
        if (const auto *lore_tag = display->getList(ItemStackBase::TAG_LORE)) {
            std::vector<std::string> lore;
            lore.reserve(lore_tag->size());
            for (auto i = 0; i < lore_tag->size(); i++) {
                lore.emplace_back(lore_tag->getString(i));
            }
            if (!lore.empty()) {
                lore_ = std::move(lore);
            }
        }
    }

    // Enchant
    enchantments_ = buildEnchantments(tag);

    // Repair cost
    if (const auto repair_cost = tag.getInt(ItemStackBase::TAG_REPAIR_COST)) {
        repair_cost_ = repair_cost;
    }

    // Unbreakable
    if (const auto unbreakable = tag.getBoolean("Unbreakable")) {
        unbreakable_ = unbreakable;
    }

    // Damage
    if (const auto damage = tag.getInt(::Item::TAG_DAMAGE)) {
        damage_ = damage;
    }
}

ItemMeta::Type EndstoneItemMeta::getType() const
{
    return Type::Item;
}

bool EndstoneItemMeta::isEmpty() const
{
    return !(hasDisplayName() || hasLore() || hasEnchants() || hasRepairCost() || isUnbreakable() || hasDamage());
}

std::unique_ptr<ItemMeta> EndstoneItemMeta::clone() const
{
    return std::make_unique<EndstoneItemMeta>(*this);
}

bool EndstoneItemMeta::hasDisplayName() const
{
    return display_name_.has_value();
}

std::optional<std::string> EndstoneItemMeta::getDisplayName() const
{
    if (!hasDisplayName()) {
        return std::nullopt;
    }
    return display_name_;
}

void EndstoneItemMeta::setDisplayName(std::optional<std::string> name)
{
    if (!name.has_value() || name.value().empty()) {
        display_name_.reset();
    }
    else {
        display_name_ = std::move(name.value());
    }
}

bool EndstoneItemMeta::hasLore() const
{
    return !lore_.empty();
}

std::optional<std::vector<std::string>> EndstoneItemMeta::getLore() const
{
    if (!hasLore()) {
        return std::nullopt;
    }
    return lore_;
}

void EndstoneItemMeta::setLore(std::optional<std::vector<std::string>> lore)
{
    if (!lore.has_value() || lore.value().empty()) {
        lore_.clear();
    }
    else {
        lore_ = std::move(lore.value());
    }
}

bool EndstoneItemMeta::hasDamage() const
{
    return damage_ > 0;
}

int EndstoneItemMeta::getDamage() const
{
    return damage_;
}

void EndstoneItemMeta::setDamage(int damage)
{
    damage_ = damage;
}

bool EndstoneItemMeta::hasEnchants() const
{
    return !enchantments_.empty();
}

bool EndstoneItemMeta::hasEnchant(const std::string &id) const
{
    return hasEnchants() && enchantments_.contains(id);
}

int EndstoneItemMeta::getEnchantLevel(const std::string &id) const
{
    if (!hasEnchant(id)) {
        return 0;
    }
    return enchantments_.at(id);
}

std::unordered_map<std::string, int> EndstoneItemMeta::getEnchants() const
{
    if (hasEnchants()) {
        return enchantments_;
    }
    return {};
}

bool EndstoneItemMeta::addEnchant(const std::string &id, int level, bool force)
{
    const auto *enchant = Enchant::getEnchantFromName(id);
    if (!enchant) {
        return false;
    }

    if (force || level >= enchant->getMinLevel() && level <= enchant->getMaxLevel()) {
        const auto old = getEnchantLevel(id);
        enchantments_[id] = level;
        return old == 0 || old != level;
    }
    return false;
}

bool EndstoneItemMeta::removeEnchant(const std::string &id)
{
    return enchantments_.erase(id) > 0;
}

void EndstoneItemMeta::removeEnchants()
{
    enchantments_.clear();
}

bool EndstoneItemMeta::hasRepairCost() const
{
    return repair_cost_ > 0;
}

int EndstoneItemMeta::getRepairCost() const
{
    return repair_cost_;
}

void EndstoneItemMeta::setRepairCost(int cost)
{
    repair_cost_ = cost;
}

bool EndstoneItemMeta::isUnbreakable() const
{
    return unbreakable_;
}

void EndstoneItemMeta::setUnbreakable(bool unbreakable)
{
    unbreakable_ = unbreakable;
}

bool EndstoneItemMeta::applicableTo(const std::string &type) const
{
    return type != "minecraft:air";
}

bool EndstoneItemMeta::equalsCommon(const EndstoneItemMeta &that) const
{
    return (hasDisplayName() ? that.hasDisplayName() && display_name_ == that.display_name_
                             : !that.hasDisplayName())                                                            //
        && (hasEnchants() ? that.hasEnchants() && enchantments_ == that.enchantments_ : !that.hasEnchants())      //
        && (lore_ == that.lore_)                                                                                  //
        && (hasRepairCost() ? that.hasRepairCost() && repair_cost_ == that.repair_cost_ : !that.hasRepairCost())  //
        && (isUnbreakable() == that.isUnbreakable())                                                              //
        && (hasDamage() ? that.hasDamage() && damage_ == that.damage_ : !that.hasDamage());                       //
}

bool EndstoneItemMeta::notUncommon() const
{
    return true;
}

void EndstoneItemMeta::applyToItem(CompoundTag &tag) const
{
    if (hasDisplayName()) {
        setDisplayTag(tag, ItemStackBase::TAG_DISPLAY_NAME, std::make_unique<StringTag>(display_name_.value()));
    }

    if (hasLore()) {
        setDisplayTag(tag, ItemStackBase::TAG_LORE, createStringList(lore_));
    }

    applyEnchantments(enchantments_, tag);

    if (hasRepairCost()) {
        tag.putInt(ItemStackBase::TAG_REPAIR_COST, repair_cost_);
    }

    if (isUnbreakable()) {
        tag.putBoolean("Unbreakable", unbreakable_);
    }

    if (hasDamage()) {
        tag.putInt(::Item::TAG_DAMAGE, damage_);
    }
}
}  // namespace endstone::core
