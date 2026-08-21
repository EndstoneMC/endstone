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

#include "endstone/core/inventory/meta/potion_meta.h"

#include "bedrock/world/item/alchemy/potion.h"
#include "endstone/check.h"

namespace endstone::core {
namespace {
constexpr auto PotionTypeNamespace = PotionType::Water.getNamespace();
}  // namespace

EndstonePotionMeta::EndstonePotionMeta(const ItemMeta *meta) : EndstoneItemMetaBase(meta)
{
    if (!meta || !meta->as<PotionMeta>()) {
        return;
    }
    const auto *potion = static_cast<const EndstonePotionMeta *>(meta);
    base_potion_type_ = potion->base_potion_type_;
}

EndstonePotionMeta::EndstonePotionMeta(const ::CompoundTag &tag) : EndstoneItemMetaBase(tag) {}

void EndstonePotionMeta::readBasePotionType(const ::ItemStackBase &item)
{
    if (const auto potion = ::Potion::getPotion(item.getAuxValue()); potion) {
        base_potion_type_ = potion->getNameId();
    }
}

bool EndstonePotionMeta::hasBasePotionType() const
{
    return !base_potion_type_.empty();
}

std::optional<PotionId> EndstonePotionMeta::getBasePotionType() const
{
    if (!hasBasePotionType()) {
        return std::nullopt;
    }
    return PotionId{PotionTypeNamespace, base_potion_type_};
}

void EndstonePotionMeta::setBasePotionType(const std::optional<PotionId> type)
{
    if (!type.has_value()) {
        base_potion_type_.clear();
        return;
    }
    const auto potion = ::Potion::getPotion(type->getKey());
    Preconditions::checkArgument(potion != nullptr, "Unknown potion type: {}", type.value());
    base_potion_type_ = potion->getNameId();
}

void EndstonePotionMeta::applyBasePotionType(::ItemStackBase &item) const
{
    if (!hasBasePotionType()) {
        return;
    }
    if (const auto potion = ::Potion::getPotion(base_potion_type_); potion) {
        item.setAuxValue(static_cast<std::int16_t>(potion->getPotionId()));
    }
}

bool EndstonePotionMeta::equalsCommon(const ItemMeta &meta) const
{
    if (!EndstoneItemMetaBase<PotionMeta>::equalsCommon(meta)) {
        return false;
    }
    if (meta.as<PotionMeta>()) {
        const auto &that = static_cast<const EndstonePotionMeta &>(meta);
        return base_potion_type_ == that.base_potion_type_;
    }
    return true;
}

bool EndstonePotionMeta::notUncommon(const ItemMeta &meta) const
{
    return EndstoneItemMetaBase::notUncommon(meta) && (meta.as<PotionMeta>() || isPotionEmpty());
}

NotNull<ItemMeta> EndstonePotionMeta::clone() const
{
    return std::make_shared<EndstonePotionMeta>(*this);
}

bool EndstonePotionMeta::isPotionEmpty() const
{
    return !hasBasePotionType();
}
}  // namespace endstone::core
