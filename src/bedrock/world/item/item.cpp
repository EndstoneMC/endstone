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

#include "bedrock/world/item/item.h"

#include "bedrock/server/commands/command_utils.h"
#include "bedrock/world/level/block/block.h"

const std::string Item::TAG_DAMAGE = "Damage";

std::int16_t Item::getId() const
{
    return id_;
}

const std::string &Item::getFullItemName() const
{
    return full_name_.getString();
}

const HashedString &Item::getFullNameHash() const
{
    return full_name_;
}

const BaseGameVersion &Item::getRequiredBaseGameVersion() const
{
    return min_required_base_game_version_;
}

const WeakPtr<BlockLegacy> &Item::getLegacyBlock() const
{
    return legacy_block_;
}

bool Item::hasTag(const ItemTag &tag) const
{
    return std::ranges::any_of(tags_, [&tag](const auto &t) { return t == tag; });
}

const std::vector<ItemTag> &Item::getTags() const
{
    return tags_;
}

Item &Item::setMinRequiredBaseGameVersion(const BaseGameVersion &base_game_version)
{
    min_required_base_game_version_ = base_game_version;
    return *this;
}

ItemDescriptor Item::buildDescriptor(std::int16_t aux_value, const CompoundTag *) const
{
    return ItemDescriptor(*this, aux_value);
}

float Item::getFurnaceBurnIntervalMultipler() const
{
    return furnace_burn_interval_modifier_;
}

const std::string &Item::getCreativeGroup() const
{
    return creative_group_;
}

CreativeItemCategory Item::getCreativeCategory() const
{
    return creative_category_;
}
