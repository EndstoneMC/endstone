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

#include "endstone/core/ability.h"

#include <format>

namespace endstone::core {

namespace {
constexpr std::array<AbilityEntry, AbilityCount> AbilityTable{{
    {Ability::AttackMobs, AbilitiesIndex::AttackMobs},
    {Ability::AttackPlayers, AbilitiesIndex::AttackPlayers},
    {Ability::Build, AbilitiesIndex::Build},
    {Ability::DoorsAndSwitches, AbilitiesIndex::DoorsAndSwitches},
    {Ability::Flying, AbilitiesIndex::Flying},
    {Ability::FlySpeed, AbilitiesIndex::FlySpeed},
    {Ability::Instabuild, AbilitiesIndex::Instabuild},
    {Ability::Invulnerable, AbilitiesIndex::Invulnerable},
    {Ability::Lightning, AbilitiesIndex::Lightning},
    {Ability::MayFly, AbilitiesIndex::MayFly},
    {Ability::Mine, AbilitiesIndex::Mine},
    {Ability::Muted, AbilitiesIndex::Muted},
    {Ability::NoClip, AbilitiesIndex::NoClip},
    {Ability::OpenContainers, AbilitiesIndex::OpenContainers},
    {Ability::OperatorCommands, AbilitiesIndex::OperatorCommands},
    {Ability::PrivilegedBuilder, AbilitiesIndex::PrivilegedBuilder},
    {Ability::Teleport, AbilitiesIndex::Teleport},
    {Ability::VerticalFlySpeed, AbilitiesIndex::VerticalFlySpeed},
    {Ability::WalkSpeed, AbilitiesIndex::WalkSpeed},
    {Ability::WorldBuilder, AbilitiesIndex::WorldBuilder},
}};
}  // namespace

EndstoneAbility::EndstoneAbility(const AbilityEntry &entry) : id_(entry.id), index_(entry.index) {}

Identifier<Ability> EndstoneAbility::getId() const
{
    return id_;
}

std::string EndstoneAbility::getTranslationKey() const
{
    return std::format("permissions.ability.{}", id_.getKey());
}

AbilitiesIndex EndstoneAbility::getIndex() const
{
    return index_;
}

const std::array<AbilityEntry, AbilityCount> &EndstoneAbility::all()
{
    return AbilityTable;
}

}  // namespace endstone::core
