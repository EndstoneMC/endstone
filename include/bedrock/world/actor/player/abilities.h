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

#include <array>

#include "abilities_index.h"
#include "ability.h"
#include "bedrock/bedrock.h"

class Abilities {
public:
    Ability &getAbility(AbilitiesIndex index)
    {
        return abilities_.at(static_cast<int>(index));
    }

    [[nodiscard]] const Ability &getAbility(AbilitiesIndex index) const
    {
        return abilities_.at(static_cast<int>(index));
    }

    [[nodiscard]] bool getBool(AbilitiesIndex index) const
    {
        const auto &ability = getAbility(index);
        return ability.isSet() ? ability.getBool() : false;
    };

    [[nodiscard]] float getFloat(AbilitiesIndex index) const
    {
        const auto &ability = getAbility(index);
        return ability.isSet() ? ability.getFloat() : 0.0F;
    };

    void setAbility(AbilitiesIndex index, bool value)
    {
        abilities_.at(static_cast<int>(index)).setBool(value);
    }

    void setAbility(AbilitiesIndex index, float value)
    {
        abilities_.at(static_cast<int>(index)).setFloat(value);
    }

    [[nodiscard]] bool isAnyAbilitySet() const
    {
        return std::any_of(abilities_.begin(), abilities_.end(), [](const auto &ability) { return ability.isSet(); });
    }

    void forEachAbility(std::function<void(Ability const &, AbilitiesIndex)> const &callback,
                        Ability::Options options) const
    {
        for (auto i = 0; i < abilities_.size(); i++) {
            const auto &ability = abilities_[i];
            if (options != Ability::Options::None) {
                if (!ability.hasOptions(options)) {
                    continue;
                }
            }
            callback(ability, static_cast<AbilitiesIndex>(i));
        }
    }

private:
    std::array<Ability, static_cast<int>(AbilitiesIndex::AbilityCount)> abilities_;
};
BEDROCK_STATIC_ASSERT_SIZE(Abilities, 228, 228);
