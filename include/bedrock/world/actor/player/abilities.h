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
#include <functional>

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

    [[nodiscard]] Type getType() const;
    [[nodiscard]] bool getBool() const;
    [[nodiscard]] float getFloat() const;
    void setBool(bool value);
    void setFloat(float value);
    [[nodiscard]] bool hasOptions(Options options) const;
    [[nodiscard]] bool isSet() const;

private:
    Type type_;
    Value value_;
    Options options_;
};
BEDROCK_STATIC_ASSERT_SIZE(Ability, 12, 12);

class Abilities {
public:
    Ability &getAbility(AbilitiesIndex index);
    [[nodiscard]] const Ability &getAbility(AbilitiesIndex index) const;
    [[nodiscard]] bool getBool(AbilitiesIndex index) const;
    [[nodiscard]] float getFloat(AbilitiesIndex index) const;
    void setAbility(AbilitiesIndex index, bool value);
    void setAbility(AbilitiesIndex index, float value);
    [[nodiscard]] bool isAnyAbilitySet() const;
    void forEachAbility(std::function<void(Ability const &, AbilitiesIndex)> const &callback,
                        Ability::Options options) const;

private:
    std::array<Ability, static_cast<int>(AbilitiesIndex::AbilityCount)> abilities_;
};
BEDROCK_STATIC_ASSERT_SIZE(Abilities, 228, 228);
