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

#include "endstone/core/inventory/meta/crossbow_meta.h"

#include "bedrock/world/item/item_instance.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {
EndstoneCrossbowMeta::EndstoneCrossbowMeta(const ItemMeta *meta) : EndstoneItemMetaBase(meta)
{
    if (!meta || !meta->as<CrossbowMeta>()) {
        return;
    }
    auto *crossbow = static_cast<const EndstoneCrossbowMeta *>(meta);
    if (crossbow->hasChargedProjectile()) {
        charged_projectile_ = crossbow->charged_projectile_;
    }
}

EndstoneCrossbowMeta::EndstoneCrossbowMeta(const ::CompoundTag &tag) : EndstoneItemMetaBase(tag)
{
    if (const auto *charged = tag.getCompound(ItemStackBase::TAG_CHARGED_ITEM)) {
        auto item = ItemInstance::fromTag(*charged);
        if (!item.isNull()) {
            charged_projectile_ = EndstoneItemStack::fromMinecraft(item);
        }
    }
}

bool EndstoneCrossbowMeta::hasChargedProjectile() const
{
    return charged_projectile_.has_value();
}

std::optional<ItemStack> EndstoneCrossbowMeta::getChargedProjectile() const
{
    return charged_projectile_;
}

void EndstoneCrossbowMeta::setChargedProjectile(std::optional<ItemStack> projectile)
{
    charged_projectile_ = std::move(projectile);
}

void EndstoneCrossbowMeta::applyToItem(::CompoundTag &tag) const
{
    EndstoneItemMetaBase::applyToItem(tag);
    tag.remove(ItemStackBase::TAG_CHARGED_ITEM);
    if (hasChargedProjectile()) {
        auto minecraft_item = EndstoneItemStack::toMinecraft(*charged_projectile_);
        if (!minecraft_item.isNull()) {
            auto compound = std::make_unique<::CompoundTag>();
            compound->putString("Name", minecraft_item.getItem()->getFullItemName());
            compound->putByte("Count", minecraft_item.getCount());
            compound->putShort("Damage", minecraft_item.getDamageValue());
            if (minecraft_item.hasUserData()) {
                compound->putCompound("tag", minecraft_item.getUserData()->clone());
            }
            tag.putCompound(ItemStackBase::TAG_CHARGED_ITEM, std::move(compound));
        }
    }
}

bool EndstoneCrossbowMeta::isEmpty() const
{
    return EndstoneItemMetaBase::isEmpty() && isCrossbowEmpty();
}

bool EndstoneCrossbowMeta::equalsCommon(const ItemMeta &meta) const
{
    if (!EndstoneItemMetaBase<CrossbowMeta>::equalsCommon(meta)) {
        return false;
    }
    if (meta.as<CrossbowMeta>()) {
        auto &that = static_cast<const EndstoneCrossbowMeta &>(meta);
        return (hasChargedProjectile() ? that.hasChargedProjectile()
                                             && charged_projectile_ == that.charged_projectile_
                                       : !that.hasChargedProjectile());
    }
    return true;
}

bool EndstoneCrossbowMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<CrossbowMeta>() || isCrossbowEmpty());
}

std::unique_ptr<ItemMeta> EndstoneCrossbowMeta::clone() const
{
    return std::make_unique<EndstoneCrossbowMeta>(*this);
}

bool EndstoneCrossbowMeta::isCrossbowEmpty() const
{
    return !hasChargedProjectile();
}
}  // namespace endstone::core
