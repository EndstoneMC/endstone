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

#include <stdexcept>

#include "bedrock/world/item/item_instance.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {
EndstoneCrossbowMeta::EndstoneCrossbowMeta(const ItemMeta *meta) : EndstoneItemMetaBase(meta)
{
    if (!meta || !meta->as<CrossbowMeta>()) {
        return;
    }
    auto *crossbow = static_cast<const EndstoneCrossbowMeta *>(meta);
    if (crossbow->hasChargedProjectiles()) {
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

bool EndstoneCrossbowMeta::hasChargedProjectiles() const
{
    return charged_projectile_.has_value();
}

std::vector<ItemStack> EndstoneCrossbowMeta::getChargedProjectiles() const
{
    if (!charged_projectile_.has_value()) {
        return {};
    }
    return {*charged_projectile_};
}

void EndstoneCrossbowMeta::setChargedProjectiles(std::vector<ItemStack> projectiles)
{
    charged_projectile_.reset();
    for (const auto &projectile : projectiles) {
        addChargedProjectile(projectile);
    }
}

void EndstoneCrossbowMeta::addChargedProjectile(const ItemStack &item)
{
    if (item.getAmount() <= 0) {
        throw std::invalid_argument("Item cannot be empty");
    }
    if (charged_projectile_.has_value()) {
        throw std::invalid_argument("A crossbow can only hold one charged projectile");
    }
    charged_projectile_ = item;
}

void EndstoneCrossbowMeta::applyToItem(::CompoundTag &tag) const
{
    EndstoneItemMetaBase::applyToItem(tag);
    tag.remove(ItemStackBase::TAG_CHARGED_ITEM);
    if (hasChargedProjectiles()) {
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
        return (hasChargedProjectiles() ? that.hasChargedProjectiles()
                                             && charged_projectile_ == that.charged_projectile_
                                       : !that.hasChargedProjectiles());
    }
    return true;
}

bool EndstoneCrossbowMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<CrossbowMeta>() || isCrossbowEmpty());
}

NotNull<ItemMeta> EndstoneCrossbowMeta::clone() const
{
    return std::make_shared<EndstoneCrossbowMeta>(*this);
}

bool EndstoneCrossbowMeta::isCrossbowEmpty() const
{
    return !hasChargedProjectiles();
}
}  // namespace endstone::core
