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

#include "endstone/core/damage/damage_source.h"

#include "endstone/core/actor/actor.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneDamageSource::EndstoneDamageSource(const ActorDamageSource &damage_source) : damage_source_(damage_source)
{
    type_ = toEndstone(damage_source.getCause());
}

std::string_view EndstoneDamageSource::getType() const
{
    return type_;
}

Actor *EndstoneDamageSource::getActor() const
{
    const auto *level = EndstoneServer::getInstance().getEndstoneLevel();
    if (const auto *actor = level->getHandle().fetchEntity(damage_source_.getEntityUniqueID(), false); actor) {
        return &actor->getEndstoneActor();
    }
    return nullptr;
}

Actor *EndstoneDamageSource::getDamagingActor() const
{
    const auto *level = EndstoneServer::getInstance().getEndstoneLevel();
    if (const auto *actor = level->getHandle().fetchEntity(damage_source_.getDamagingEntityUniqueID(), false); actor) {
        return &actor->getEndstoneActor();
    }
    return nullptr;
}

bool EndstoneDamageSource::isIndirect() const
{
    return getActor() != getDamagingActor();
}

std::string_view EndstoneDamageSource::toEndstone(ActorDamageCause cause)
{
    // https://learn.microsoft.com/en-us/minecraft/creator/reference/content/addonsreference/examples/addonentities
    static std::unordered_map<ActorDamageCause, std::string_view> causes = {
        {ActorDamageCause::None, "none"},
        {ActorDamageCause::Override, "override"},
        {ActorDamageCause::Contact, "contact"},
        {ActorDamageCause::EntityAttack, "entity_attack"},
        {ActorDamageCause::Projectile, "projectile"},
        {ActorDamageCause::Suffocation, "suffocation"},
        {ActorDamageCause::Fall, "fall"},
        {ActorDamageCause::Fire, "fire"},
        {ActorDamageCause::FireTick, "fire_tick"},
        {ActorDamageCause::Lava, "lava"},
        {ActorDamageCause::Drowning, "drowning"},
        {ActorDamageCause::BlockExplosion, "block_explosion"},
        {ActorDamageCause::EntityExplosion, "entity_explosion"},
        {ActorDamageCause::Void, "void"},
        {ActorDamageCause::SelfDestruct, "self_destruct"},
        {ActorDamageCause::Magic, "magic"},
        {ActorDamageCause::Wither, "wither"},
        {ActorDamageCause::Starve, "starve"},
        {ActorDamageCause::Anvil, "anvil"},
        {ActorDamageCause::Thorns, "thorns"},
        {ActorDamageCause::FallingBlock, "falling_block"},
        {ActorDamageCause::Piston, "piston"},
        {ActorDamageCause::FlyIntoWall, "fly_into_wall"},
        {ActorDamageCause::Magma, "magma"},
        {ActorDamageCause::Fireworks, "fireworks"},
        {ActorDamageCause::Lightning, "lightning"},
        {ActorDamageCause::Charging, "charging"},
        {ActorDamageCause::Temperature, "temperature"},
        {ActorDamageCause::Freezing, "freezing"},
        {ActorDamageCause::Stalactite, "stalactite"},
        {ActorDamageCause::Stalagmite, "stalagmite"},
        {ActorDamageCause::RamAttack, "ram_attack"},
        {ActorDamageCause::SonicBoom, "sonic_boom"},
        {ActorDamageCause::Campfire, "campfire"},
        {ActorDamageCause::SoulCampfire, "soul_campfire"},
        {ActorDamageCause::MaceSmash, "mace_smash"},
        // {ActorDamageCause::Dehydration, "dehydration"},
        {ActorDamageCause::All, "all"},
    };
    if (!causes.contains(cause)) {
        throw std::runtime_error("Unknown damage cause: " +
                                 std::to_string(static_cast<std::underlying_type_t<ActorDamageCause>>(cause)));
    }
    return causes.at(cause);
}

}  // namespace endstone::core
