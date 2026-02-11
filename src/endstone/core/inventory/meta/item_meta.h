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

#include <memory>
#include <string>
#include <utility>

#include "bedrock/world/item/enchanting/enchant.h"
#include "bedrock/world/item/item.h"
#include "endstone/core/nbt.h"
#include "endstone/inventory/item_type.h"
#include "endstone/inventory/meta/item_meta.h"

namespace endstone::core {
class ItemMetaExtras {
public:
    virtual ~ItemMetaExtras() = default;
    [[nodiscard]] virtual bool isEmpty() const = 0;
    virtual void applyToItem(::CompoundTag &tag) const = 0;
    [[nodiscard]] virtual bool applicableTo(ItemTypeId type) const = 0;
    [[nodiscard]] virtual bool equalsCommon(const ItemMeta &meta) const = 0;
    [[nodiscard]] virtual bool notUncommon(const ItemMeta &meta) const = 0;
};

template <typename Interface>
    requires std::is_base_of_v<ItemMeta, Interface>
class EndstoneItemMetaBase : public Interface, public ItemMetaExtras {
public:
    explicit EndstoneItemMetaBase(const ItemMeta *meta)
    {
        if (meta == nullptr) {
            return;
        }
        auto *m = static_cast<const EndstoneItemMetaBase *>(meta);
        display_name_ = m->display_name_;
        lore_ = m->lore_;
        enchantments_ = m->enchantments_;
        repair_cost_ = m->repair_cost_;
        damage_ = m->damage_;
        unbreakable_ = m->unbreakable_;
    }

    explicit EndstoneItemMetaBase(const ::CompoundTag &tag)
    {
        if (const auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
            if (const auto &display_name = display->getString(ItemStackBase::TAG_DISPLAY_NAME); !display_name.empty()) {
                display_name_ = display_name;
            }
            if (const auto *lines = display->getList(ItemStackBase::TAG_LORE)) {
                lore_.clear();
                for (auto i = 0; i < lines->size(); i++) {
                    lore_.emplace_back(lines->getString(i));
                }
            }
        }
        if (const auto *enchants = tag.getList(ItemStackBase::TAG_ENCHANTS)) {
            enchantments_ = buildEnchantments(*enchants);
        }
        if (const auto repair_cost = tag.getInt(ItemStackBase::TAG_REPAIR_COST)) {
            repair_cost_ = repair_cost;
        }
        if (const auto unbreakable = tag.getBoolean(ItemStackBase::TAG_UNBREAKABLE)) {
            unbreakable_ = unbreakable;
        }
        if (const auto damage = tag.getInt(::Item::TAG_DAMAGE)) {
            damage_ = damage;
        }
    }

    [[nodiscard]] ItemMeta::Type getType() const override { return Interface::MetaType; }

    [[nodiscard]] bool hasDisplayName() const override { return !display_name_.empty(); }

    [[nodiscard]] std::string getDisplayName() const override { return display_name_; }

    void setDisplayName(std::optional<std::string> name) override { display_name_ = std::move(name.value_or("")); }

    [[nodiscard]] bool hasLore() const override { return !lore_.empty(); }

    [[nodiscard]] std::vector<std::string> getLore() const override { return lore_; }

    void setLore(std::optional<std::vector<std::string>> lore) override
    {
        if (!lore.has_value() || lore.value().empty()) {
            lore_.clear();
        }
        else {
            lore_ = std::move(lore.value());
        }
    }

    [[nodiscard]] bool hasEnchants() const override { return !enchantments_.empty(); }

    [[nodiscard]] bool hasEnchant(EnchantmentId id) const override
    {
        if (!hasEnchants()) {
            return false;
        }
        return enchantments_.contains(id);
    }

    [[nodiscard]] int getEnchantLevel(EnchantmentId id) const override
    {
        if (!hasEnchant(id)) {
            return 0;
        }
        return enchantments_.at(id);
    }

    [[nodiscard]] std::unordered_map<const Enchantment *, int> getEnchants() const override
    {
        if (hasEnchants()) {
            std::unordered_map<const Enchantment *, int> enchants;
            for (const auto &[id, lvl] : enchantments_) {
                enchants.emplace(Enchantment::get(id), lvl);
            }
            return enchants;
        }
        return {};
    }

    bool addEnchant(EnchantmentId id, int level, bool force) override
    {
        const auto *ench = Enchantment::get(id);
        if (!ench) {
            return false;
        }
        if (force || level >= ench->getStartLevel() && level <= ench->getMaxLevel()) {
            const auto old = getEnchantLevel(id);
            enchantments_[id] = level;
            return old == 0 || old != level;
        }
        return false;
    }

    bool removeEnchant(EnchantmentId id) override { return enchantments_.erase(id) > 0; }

    void removeEnchants() override { enchantments_.clear(); }

    [[nodiscard]] bool hasConflictingEnchant(EnchantmentId id) const override
    {
        const auto *ench = Enchantment::get(id);
        if (!ench || enchantments_.empty()) {
            return false;
        }
        for (const auto &key : enchantments_ | std::views::keys) {
            const auto *enchant = Enchantment::get(key);
            if (enchant->conflictsWith(*ench)) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] bool isUnbreakable() const override { return unbreakable_; }

    void setUnbreakable(bool unbreakable) override { unbreakable_ = unbreakable; }

    [[nodiscard]] bool hasDamage() const override { return damage_ > 0; }

    [[nodiscard]] int getDamage() const override { return damage_; }

    void setDamage(int damage) override { damage_ = damage; }

    [[nodiscard]] bool hasRepairCost() const override { return repair_cost_ > 0; }

    [[nodiscard]] int getRepairCost() const override { return repair_cost_; }

    void setRepairCost(int cost) override { repair_cost_ = cost; }

    [[nodiscard]] CompoundTag toNbt() const override
    {
        auto tag = std::make_unique<::CompoundTag>();
        applyToItem(*tag);
        return nbt::fromMinecraft(*tag).get<CompoundTag>();
    }

    [[nodiscard]] bool isEmpty() const override
    {
        return !(hasDisplayName() || hasLore() || hasEnchants() || hasRepairCost() || isUnbreakable() || hasDamage());
    }

    void applyToItem(::CompoundTag &tag) const override
    {
        if (auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
            display->remove(ItemStackBase::TAG_DISPLAY_NAME);
            display->remove(ItemStackBase::TAG_LORE);
        }
        if (hasDisplayName()) {
            setDisplayTag(tag, ItemStackBase::TAG_DISPLAY_NAME, std::make_unique<::StringTag>(getDisplayName()));
        }
        if (hasLore()) {
            setDisplayTag(tag, ItemStackBase::TAG_LORE, createStringList(getLore()));
        }

        applyEnchantments(enchantments_, tag);

        tag.remove(ItemStackBase::TAG_REPAIR_COST);
        if (hasRepairCost()) {
            tag.putInt(ItemStackBase::TAG_REPAIR_COST, getRepairCost());
        }

        tag.remove(ItemStackBase::TAG_UNBREAKABLE);
        if (isUnbreakable()) {
            tag.putBoolean(ItemStackBase::TAG_UNBREAKABLE, isUnbreakable());
        }

        tag.remove(::Item::TAG_DAMAGE);
        if (hasDamage()) {
            tag.putInt(::Item::TAG_DAMAGE, getDamage());
        }
    }

    [[nodiscard]] bool applicableTo(ItemTypeId type) const override { return type != ItemType::Air; }

    [[nodiscard]] bool equalsCommon(const ItemMeta &meta) const override
    {
        const auto &that = static_cast<const EndstoneItemMetaBase &>(meta);
        return (hasDisplayName() ? that.hasDisplayName() && display_name_ == that.display_name_
                                 : !that.hasDisplayName())                                                            //
            && (hasEnchants() ? that.hasEnchants() && enchantments_ == that.enchantments_ : !that.hasEnchants())      //
            && (lore_ == that.lore_)                                                                                  //
            && (hasRepairCost() ? that.hasRepairCost() && repair_cost_ == that.repair_cost_ : !that.hasRepairCost())  //
            && (isUnbreakable() == that.isUnbreakable())                                                              //
            && (hasDamage() ? that.hasDamage() && damage_ == that.damage_ : !that.hasDamage());                       //
    }

    [[nodiscard]] bool notUncommon(const ItemMeta &meta) const override { return true; }

private:
    static std::unique_ptr<::ListTag> createStringList(const std::vector<std::string> &list)
    {
        auto list_tag = std::make_unique<::ListTag>();
        for (const auto &value : list) {
            list_tag->add(std::make_unique<::StringTag>(value));
        }
        return list_tag;
    }

    static void setDisplayTag(::CompoundTag &tag, std::string key, std::unique_ptr<::Tag> value)
    {
        auto *display = tag.getCompound(ItemStackBase::TAG_DISPLAY);
        if (!display) {
            display = tag.putCompound(ItemStackBase::TAG_DISPLAY, std::make_unique<::CompoundTag>());
        }
        display->put(std::move(key), std::move(value));
    }

    static std::unordered_map<EnchantmentId, int> buildEnchantments(const ::ListTag &tag)
    {
        std::unordered_map<EnchantmentId, int> enchantments;
        tag.forEachCompoundTag([&](const ::CompoundTag &enchant_tag) {
            auto id = enchant_tag.getShort("id");
            auto lvl = enchant_tag.getShort("lvl") & 0xffff;
            const auto *enchant = Enchant::getEnchant(static_cast<Enchant::Type>(id));
            if (enchant) {
                enchantments[enchant->getStringId().getString()] = lvl;
            }
        });
        return enchantments;
    }

    static void applyEnchantments(const std::unordered_map<EnchantmentId, int> &enchantments, ::CompoundTag &tag)
    {
        tag.remove(ItemStackBase::TAG_ENCHANTS);
        if (enchantments.empty()) {
            return;
        }

        auto list = std::make_unique<::ListTag>();
        for (const auto &[id, lvl] : enchantments) {
            auto subtag = std::make_unique<::CompoundTag>();
            const auto *enchant = Enchant::getEnchantFromName(std::string(EnchantmentId(id).getKey()));
            if (!enchant) {
                continue;
            }
            subtag->putShort("id", static_cast<std::int16_t>(enchant->getEnchantType()));
            subtag->putShort("lvl", static_cast<std::int16_t>(lvl));
            list->add(std::move(subtag));
        }
        tag.put(ItemStackBase::TAG_ENCHANTS, std::move(list));
    }

    std::string display_name_;
    std::vector<std::string> lore_;
    std::unordered_map<EnchantmentId, int> enchantments_;
    int repair_cost_ = 0;
    int damage_ = 0;
    bool unbreakable_ = false;
};

class EndstoneItemMeta : public EndstoneItemMetaBase<ItemMeta> {
public:
    using Base = EndstoneItemMetaBase;
    using Base::Base;

    [[nodiscard]] std::unique_ptr<ItemMeta> clone() const override { return std::make_unique<EndstoneItemMeta>(*this); }
};
}  // namespace endstone::core
