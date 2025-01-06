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

#include "bedrock/world/actor/player/abilities.h"

#include <entt/entt.hpp>

Ability::Type Ability::getType() const
{
    return type_;
}

bool Ability::getBool() const
{
    return value_.bool_val;
}

float Ability::getFloat() const
{
    return value_.float_val;
}

void Ability::setBool(bool value)
{
    if (type_ == Type::Unset) {
        type_ = Type::Bool;
        value_ = {};
    }
    value_.bool_val = value;
}

void Ability::setFloat(float value)
{
    if (type_ == Type::Unset) {
        type_ = Type::Float;
        value_ = {};
    }
    value_.float_val = value;
}

bool Ability::hasOptions(Options options) const
{
    return !!(options & options_);
}

bool Ability::isSet() const
{
    return type_ != Type::Unset;
}

Ability &Abilities::getAbility(AbilitiesIndex index)
{
    return abilities_.at(static_cast<int>(index));
}

const Ability &Abilities::getAbility(AbilitiesIndex index) const
{
    return abilities_.at(static_cast<int>(index));
}

bool Abilities::getBool(AbilitiesIndex index) const
{
    const auto &ability = getAbility(index);
    return ability.isSet() ? ability.getBool() : false;
}

float Abilities::getFloat(AbilitiesIndex index) const
{
    const auto &ability = getAbility(index);
    return ability.isSet() ? ability.getFloat() : 0.0F;
}

void Abilities::setAbility(AbilitiesIndex index, bool value)
{
    abilities_.at(static_cast<int>(index)).setBool(value);
}

void Abilities::setAbility(AbilitiesIndex index, float value)
{
    abilities_.at(static_cast<int>(index)).setFloat(value);
}

bool Abilities::isAnyAbilitySet() const
{
    return std::any_of(abilities_.begin(), abilities_.end(), [](const auto &ability) { return ability.isSet(); });
}

void Abilities::forEachAbility(std::function<void(Ability const &, AbilitiesIndex)> const &callback,
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
