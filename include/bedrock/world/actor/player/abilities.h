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

#include <entt/entt.hpp>

#include "bedrock/bedrock.h"

enum class AbilitiesIndex : std::int16_t {
    Invalid = -1,
    Build = 0,
    Mine = 1,
    DoorsAndSwitches = 2,
    OpenContainers = 3,
    AttackPlayers = 4,
    AttackMobs = 5,
    OperatorCommands = 6,
    Teleport = 7,
    Invulnerable = 8,
    Flying = 9,
    MayFly = 10,
    Instabuild = 11,
    Lightning = 12,
    FlySpeed = 13,
    WalkSpeed = 14,
    Muted = 15,
    WorldBuilder = 16,
    NoClip = 17,
    PrivilegedBuilder = 18,
    AbilityCount = 19,
};

class Ability {
public:
    enum class Type : std::uint8_t {
        Invalid = 0,
        Unset = 1,
        Bool = 2,
        Float = 3,
    };

    enum class Options : std::uint8_t {
        None = 0,
        NoSave = 1,
        CommandExposed = 2,
        PermissionsInterfaceExposed = 4,
        _entt_enum_as_bitmask  // NOLINT(*-identifier-naming)
    };

    union Value {
        bool bool_val;
        float float_val;
    };

    Ability() = default;

    [[nodiscard]] Type getType() const
    {
        return type_;
    }

    [[nodiscard]] bool getBool() const
    {
        return value_.bool_val;
    }

    [[nodiscard]] float getFloat() const
    {
        return value_.float_val;
    }

    void setBool(bool value)
    {
        if (type_ == Type::Unset) {
            type_ = Type::Bool;
            value_ = {};
        }
        value_.bool_val = value;
    }

    void setFloat(float value)
    {
        if (type_ == Type::Unset) {
            type_ = Type::Float;
            value_ = {};
        }
        value_.float_val = value;
    }

    [[nodiscard]] bool hasOptions(Options options) const
    {
        return !!(options & options_);
    }

    [[nodiscard]] bool isSet() const
    {
        return type_ != Type::Unset;
    }

private:
    Type type_;
    Value value_;
    Options options_;
};
BEDROCK_STATIC_ASSERT_SIZE(Ability, 12, 12);

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
