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

#include "endstone/core/inventory/item_factory.h"

#include "bedrock/world/item/item.h"

namespace endstone::core {

template <typename T>
void loadData(T &meta, const CompoundTag &tag);

template <>
void loadData(ItemMeta &meta, const CompoundTag &tag)
{
    if (const auto *display_tag = tag.getCompound(ItemStackBase::TAG_DISPLAY)) {
        // Display name
        auto display_name = display_tag->getString(ItemStackBase::TAG_DISPLAY_NAME);
        if (!display_name.empty()) {
            meta.setDisplayName(display_name);
        }

        // Lore
        if (const auto *lore_tag = display_tag->getList(ItemStackBase::TAG_LORE)) {
            std::vector<std::string> lore;
            lore.reserve(lore_tag->size());
            for (auto i = 0; i < lore_tag->size(); i++) {
                lore.emplace_back(lore_tag->getString(i));
            }
            if (!lore.empty()) {
                meta.setLore(lore);
            }
        }
    }

    // Damage
    if (const auto damage = tag.getInt(Item::TAG_DAMAGE)) {
        meta.setDamage(damage);
    }

    // Enchant
    if (const auto *const enchants = tag.getList(ItemStackBase::TAG_ENCH)) {
        enchants->forEachCompoundTag(
            [&](const CompoundTag &enchant) { meta.addEnchant(enchant.getShort("id"), enchant.getShort("lvl")); });
    }
}

template <>
void loadData(MapMeta &meta, const CompoundTag &tag)
{
    loadData<ItemMeta>(meta, tag);
    // TODO(item): implement this
}

std::unique_ptr<ItemMeta> EndstoneItemFactory::getItemMeta(const std::string_view type, const ItemStackBase &item)
{
    auto meta = ItemFactory::getItemMeta(type);
    const auto *tag = item.getUserData();
    if (!tag) {
        return meta;
    }

    switch (meta->getType()) {
    case ItemMeta::Type::Map:
        loadData<MapMeta>(static_cast<MapMeta &>(*meta), *tag);
        break;
    default:
        loadData<ItemMeta>(*meta, *tag);
        break;
    }
    return meta;
}

template <typename T>
void applyTo(const T &meta, CompoundTag &tag);

template <>
void applyTo(const ItemMeta &meta, CompoundTag &tag)
{
    auto *display_tag = tag.getCompound(ItemStackBase::TAG_DISPLAY);
    if (!display_tag) {
        display_tag = tag.putCompound(ItemStackBase::TAG_DISPLAY, std::make_unique<CompoundTag>());
    }

    // Display name
    if (const auto display_name = meta.getDisplayName(); display_name.has_value()) {
        display_tag->put(ItemStackBase::TAG_DISPLAY_NAME, std::make_unique<StringTag>(display_name.value()));
    }
    else {
        display_tag->remove(ItemStackBase::TAG_DISPLAY_NAME);
    }

    // Lore
    if (const auto lore = meta.getLore(); lore.has_value()) {
        auto lore_tag = std::make_unique<ListTag>();
        for (const auto &line : lore.value()) {
            lore_tag->add(std::make_unique<StringTag>(line));
        }
        display_tag->put(ItemStackBase::TAG_LORE, std::move(lore_tag));
    }
    else {
        display_tag->remove(ItemStackBase::TAG_LORE);
    }

    if (meta.hasDamage()) {
        tag.putInt(Item::TAG_DAMAGE, meta.getDamage());
    }
    else {
        tag.remove(Item::TAG_DAMAGE);
    }

    // Enchant
    if (meta.hasEnchants()) {
        auto enchants = meta.getEnchants();
        auto enchants_tag = std::make_unique<ListTag>();
        for (const auto &[id, lvl] : enchants) {
            auto enchant = std::make_unique<CompoundTag>();
            enchant->putShort("id", id);
            enchant->putShort("lvl", lvl);
            enchants_tag->add(std::move(enchant));
        }
        tag.put(ItemStackBase::TAG_ENCH, std::move(enchants_tag));
    }
}

template <>
void applyTo(const MapMeta &meta, CompoundTag &tag)
{
    applyTo<ItemMeta>(meta, tag);
    // TODO(item): implement this
}

void EndstoneItemFactory::applyToItem(const ItemMeta &meta, ItemStackBase &item)
{
    if (!item.hasUserData()) {
        item.setUserData(std::make_unique<CompoundTag>());
    }
    auto *tag = item.getUserData();
    switch (meta.getType()) {
    case ItemMeta::Type::Map:
        applyTo<MapMeta>(static_cast<const MapMeta &>(meta), *tag);
        break;
    default:
        applyTo<ItemMeta>(meta, *tag);
        break;
    }
}
}  // namespace endstone::core
