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

#include "endstone/core/attribute/attribute.h"

#include <array>
#include <stdexcept>

namespace endstone::core {

namespace {
constexpr std::array AttributeTable{
    Attribute::Health,
    Attribute::FollowRange,
    Attribute::KnockbackResistance,
    Attribute::MovementSpeed,
    Attribute::UnderwaterMovementSpeed,
    Attribute::LavaMovementSpeed,
    Attribute::AttackDamage,
    Attribute::Absorption,
    Attribute::Luck,
    Attribute::JumpStrength,
    AttributeId::minecraft("friction_modifier"),
    AttributeId::minecraft("bounciness"),
    AttributeId::minecraft("air_drag_modifier"),
    Attribute::PlayerHunger,
    Attribute::PlayerSaturation,
    Attribute::PlayerExhaustion,
    Attribute::PlayerLevel,
    Attribute::PlayerExperience,
    Attribute::ZombieSpawnReinforcements,
};
}  // namespace

EndstoneAttribute::EndstoneAttribute(AttributeId id) : id_(id) {}

AttributeId EndstoneAttribute::getId() const
{
    return id_;
}

std::string EndstoneAttribute::getTranslationKey() const
{
    throw std::runtime_error("Bedrock attributes carry no translation key.");
}

std::span<const AttributeId> EndstoneAttribute::all()
{
    return AttributeTable;
}

}  // namespace endstone::core
