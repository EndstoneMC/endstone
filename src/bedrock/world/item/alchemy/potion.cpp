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

#include "bedrock/world/item/alchemy/potion.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

#include "bedrock/locale/i18n.h"
#include "bedrock/symbol.h"

namespace {
constexpr std::string_view PotionTypePrefix = "minecraft:potion_type:";
}  // namespace

std::shared_ptr<const Potion> Potion::getPotion(const int potion_id)
{
    if (static_cast<unsigned int>(potion_id) >= MAX_POTIONS) {
        return nullptr;
    }
    // Endstone: Potion::mPotionsById[MAX_POTIONS] is the BDS registry global, indexed by potion id.
    static auto *potions = BEDROCK_VAR(std::shared_ptr<const Potion> *, "Potion::mPotionsById");
    return potions[potion_id];
}

std::shared_ptr<const Potion> Potion::getPotion(const std::string_view potion_name_id)
{
    std::string name{potion_name_id};
    std::ranges::transform(name, name.begin(),
                           [](const unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (const auto pos = name.find(PotionTypePrefix); pos != std::string::npos) {
        name.erase(pos, PotionTypePrefix.size());
    }

    // Endstone: BDS keeps its own mPotionsByName hash map; we build the equivalent once from the registry global.
    static const auto potions_by_name = [] {
        std::unordered_map<std::string, int> map;
        for (auto id = 0; id < MAX_POTIONS; ++id) {
            if (const auto potion = getPotion(id); potion) {
                map.emplace(potion->getNameId(), id);
            }
        }
        return map;
    }();

    if (const auto it = potions_by_name.find(name); it != potions_by_name.end()) {
        return getPotion(it->second);
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Potion>> Potion::getPotions()
{
    std::vector<std::shared_ptr<const Potion>> potions;
    for (auto id = 0; id < MAX_POTIONS; ++id) {
        if (auto potion = getPotion(id); potion) {
            potions.push_back(std::move(potion));
        }
    }
    return potions;
}

std::string Potion::getDescriptionId(const PotionType potion_type) const
{
    std::string legacy;
    if (description_ids_.empty()) {
        legacy = prefix_.empty() ? "%item.emptyPotion.name" : "%potion.prefix." + prefix_ + " %item.potion.name";
    }
    else {
        legacy = prefix_.empty() ? "%potion." + description_ids_.front() + ".postfix"
                                 : "%potion.prefix." + prefix_ + " %potion." + description_ids_.front() + ".postfix";
    }
    switch (potion_type) {
    case Lingering:
        legacy = "%potion.prefix.linger " + legacy;
        break;
    case Splash:
        legacy = "%potion.prefix.grenade " + legacy;
        break;
    default:
        break;
    }

    auto stripped = legacy;
    std::erase(stripped, '%');
    if (getI18n().get(legacy, nullptr) != stripped) {
        return legacy;
    }

    std::string result = "%potion.";
    if (prefix_.empty() && description_ids_.empty()) {
        result += "emptyPotion";
    }
    else {
        result += prefix_.empty() ? description_ids_.front() : prefix_;
    }
    switch (potion_type) {
    case Lingering:
        result += ".linger";
        break;
    case Splash:
        result += ".splash";
        break;
    default:
        break;
    }
    return result + ".name";
}

std::string Potion::getDescriptionId() const
{
    if (description_ids_.empty()) {
        return {};
    }
    return description_ids_.front();
}
