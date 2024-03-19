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

#include <bitset>

#include "bedrock/world/actor/actor.h"

class Mob : public Actor {
public:
    ~Mob() override = 0;
    virtual void knockback(Actor *, int, float, float, float, float, float) = 0;
    virtual void spawnAnim() = 0;
    virtual void setSprinting(bool) = 0;
    virtual Puv::Legacy::LevelSoundEvent getHurtSound() = 0;
    virtual Puv::Legacy::LevelSoundEvent getDeathSound() = 0;
    [[nodiscard]] virtual float getSpeed() const = 0;
    virtual void setSpeed(float) = 0;
    virtual void hurtEffects(ActorDamageSource const &, float, bool, bool) = 0;
    virtual void aiStep() = 0;
    virtual void pushActors() = 0;
    virtual bool checkSpawnRules(bool) = 0;
    [[nodiscard]] virtual bool checkSpawnObstruction() const = 0;
    [[nodiscard]] virtual int getItemUseDuration() const = 0;
    [[nodiscard]] virtual float getItemUseStartupProgress() const = 0;
    [[nodiscard]] virtual float getItemUseIntervalProgress() const = 0;
    virtual float getMaxHeadXRot() = 0;
    virtual bool isAlliedTo(Mob *) const = 0;
    virtual bool doHurtTarget(Actor *, ActorDamageCause const &) = 0;
    virtual int getArmorValue() = 0;
    virtual void hurtArmorSlots(ActorDamageSource const &, int, std::bitset<4>) = 0;
    virtual void setDamagedArmor(ArmorSlot, ItemStack const &) = 0;
    virtual void sendArmorDamage(std::bitset<4>) = 0;
    virtual void sendArmor(std::bitset<4>) = 0;
    [[nodiscard]] virtual std::vector<ItemStack const *> getAllHand() const = 0;
    [[nodiscard]] virtual std::vector<ItemStack const *> getAllEquipment() const = 0;
    virtual void dropEquipmentOnDeath() = 0;
    virtual void dropEquipmentOnDeath(ActorDamageSource const &, int) = 0;
    virtual void clearVanishEnchantedItemsOnDeath() = 0;
    virtual void sendInventory(bool) = 0;
    [[nodiscard]] virtual float getDamageAfterEnchantReduction(ActorDamageSource const &, float) const = 0;
    virtual bool createAIGoals() = 0;
    virtual void onBorn(Actor &, Actor &) = 0;
    virtual bool setItemSlot(EquipmentSlot, ItemStack const &) = 0;
    virtual void setTransitioningSitting(bool) = 0;

protected:
    [[nodiscard]] virtual float _getWalkTargetValue(BlockPos const &) const = 0;

public:
    virtual bool canExistWhenDisallowMob() = 0;
    virtual std::unique_ptr<BodyControl> initBodyControl() = 0;
    virtual void newServerAiStep() = 0;
    virtual void tickDeath() = 0;

protected:
    [[nodiscard]] virtual AABB _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &) const = 0;
};
