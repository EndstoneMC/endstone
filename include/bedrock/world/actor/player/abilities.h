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
        return abilities_.at(static_cast<std::size_t>(index));
    }

    [[nodiscard]] const Ability &getAbility(AbilitiesIndex index) const
    {
        return abilities_.at(static_cast<std::size_t>(index));
    }

    void setAbility(AbilitiesIndex index, bool value)
    {
        abilities_.at(static_cast<std::size_t>(index)).setBool(value);
    }

    void setAbility(AbilitiesIndex index, float value)
    {
        abilities_.at(static_cast<std::size_t>(index)).setFloat(value);
    }

private:
    std::array<Ability, static_cast<std::size_t>(AbilitiesIndex::AbilityCount)> abilities_;
};
BEDROCK_STATIC_ASSERT_SIZE(Abilities, 228, 228);
