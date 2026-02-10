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

#include "bedrock/world/actor/actor_damage_source.h"

#include <utility>

#include "actor.h"

ActorDamageSource::ActorDamageSource(ActorDamageCause cause, std::string death_message_override)
    : cause_(cause), death_message_override_(std::move(death_message_override))
{
}

ActorDamageCause ActorDamageSource::getCause() const
{
    return cause_;
}

void ActorDamageSource::setCause(ActorDamageCause cause)
{
    cause_ = cause;
}

bool ActorDamageSource::isEntitySource() const
{
    return false;
}

bool ActorDamageSource::isChildEntitySource() const
{
    return false;
}

bool ActorDamageSource::isBlockSource() const
{
    return false;
}

bool ActorDamageSource::isFire() const
{
    switch (cause_) {
    case ActorDamageCause::Fire:
    case ActorDamageCause::FireTick:
    case ActorDamageCause::Lava:
    case ActorDamageCause::Magma:
        return true;
    default:
        return false;
    }
}

bool ActorDamageSource::isReducedByResistanceEffect() const
{
    switch (cause_) {
    case ActorDamageCause::Override:
    case ActorDamageCause::Void:
    case ActorDamageCause::SelfDestruct:
    case ActorDamageCause::Starve:
        return false;
    default:
        return true;
    }
}

bool ActorDamageSource::isReducedByEnchantReduction() const
{
    switch (cause_) {
    case ActorDamageCause::SonicBoom:
        return false;
    default:
        return true;
    }
}

bool ActorDamageSource::isReducedByArmorReduction() const
{
    if (isFallDamage()) {
        return false;
    }
    switch (cause_) {
    case ActorDamageCause::Override:
    case ActorDamageCause::Suffocation:
    case ActorDamageCause::FireTick:
    case ActorDamageCause::Drowning:
    case ActorDamageCause::Void:
    case ActorDamageCause::SelfDestruct:
    case ActorDamageCause::Magic:
    case ActorDamageCause::Wither:
    case ActorDamageCause::Starve:
    case ActorDamageCause::FlyIntoWall:
    case ActorDamageCause::Freezing:
    case ActorDamageCause::SonicBoom:
        return false;
    default:
        return true;
    }
}

bool ActorDamageSource::isFallingBlockDamage() const
{
    switch (cause_) {
    case ActorDamageCause::Anvil:
    case ActorDamageCause::FallingBlock:
    case ActorDamageCause::Stalactite:
        return true;
    default:
        return false;
    }
}

bool ActorDamageSource::isFallDamage() const
{
    switch (cause_) {
    case ActorDamageCause::Fall:
    case ActorDamageCause::Stalagmite:
        return true;
    default:
        return false;
    }
}

std::pair<std::string, std::vector<std::string>> ActorDamageSource::getDeathMessage(std::string dead_name,
                                                                                    Actor *dead) const
{
    std::pair<std::string, std::vector<std::string>> result;
    switch (cause_) {
    case ActorDamageCause::Suffocation: {
        result.first = "death.attack.inWall";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Fall: {
        if (dead && dead->getFallDistance() > 5.0F) {
            result.first = "death.fell.accident.generic";
        }
        else {
            result.first = "death.attack.fall";
        }
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Fire: {
        result.first = "death.attack.inFire";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::FireTick: {
        result.first = "death.attack.onFire";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Lava: {
        result.first = "death.attack.lava";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Drowning: {
        result.first = "death.attack.drown";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::BlockExplosion: {
        result.first = "death.attack.explosion.by.bed";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::EntityExplosion: {
        result.first = "death.attack.explosion";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Void: {
        result.first = "death.attack.outOfWorld";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Magic: {
        result.first = "death.attack.magic";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Wither: {
        result.first = "death.attack.wither";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Starve: {
        result.first = "death.attack.starve";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Anvil: {
        result.first = "death.attack.anvil";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::FlyIntoWall: {
        result.first = "death.attack.flyIntoWall";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Fireworks: {
        result.first = "death.attack.fireworks";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Lightning: {
        result.first = "death.attack.lightningBolt";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Freezing: {
        result.first = "death.attack.freeze";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Stalactite: {
        result.first = "death.attack.stalactite";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::Stalagmite: {
        result.first = "death.attack.stalagmite";
        result.second = {std::move(dead_name)};
        break;
    }
    case ActorDamageCause::SonicBoom: {
        result.first = "death.attack.sonicBoom";
        result.second = {std::move(dead_name)};
        break;
    }
    default: {
        result.first = "death.attack.generic";
        result.second = {std::move(dead_name)};
        break;
    }
    }
    return result;
}

bool ActorDamageSource::getIsCreative() const
{
    return false;
}

bool ActorDamageSource::getIsWorldBuilder() const
{
    return false;
}

ActorUniqueID ActorDamageSource::getEntityUniqueID() const
{
    return ActorUniqueID::INVALID_ID;
}

ActorType ActorDamageSource::getEntityType() const
{
    return ActorType::Undefined;
}

ActorCategory ActorDamageSource::getEntityCategories() const
{
    return ActorCategory::None;
}

bool ActorDamageSource::getDamagingEntityIsCreative() const
{
    return false;
}

bool ActorDamageSource::getDamagingEntityIsWorldBuilder() const
{
    return false;
}

ActorUniqueID ActorDamageSource::getDamagingEntityUniqueID() const
{
    return ActorUniqueID::INVALID_ID;
}

ActorType ActorDamageSource::getDamagingEntityType() const
{
    return ActorType::Undefined;
}

ActorCategory ActorDamageSource::getDamagingEntityCategories() const
{
    return ActorCategory::None;
}

std::unique_ptr<ActorDamageSource> ActorDamageSource::clone() const
{
    return std::make_unique<ActorDamageSource>(*this);
}

const Block &ActorDamageByBlockSource::getBlock() const
{
    return *block_;
}
