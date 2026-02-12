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

#include "bedrock/forward.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/item/clock_sprite_calculator.h"
#include "bedrock/world/item/compass_sprite_calculator.h"

enum class MobSpawnMethod : std::uint8_t {
    Unknown = 0,
    SpawnEgg = 1,
    Command = 2,
    Dispenser = 3,
    Spawner = 4,
    SpawnMethod_Count = 5,
};

struct BuiltInMobComponents {
    gsl::not_null<DeathTickingComponent *> death_ticking_component;
    gsl::not_null<MobAnimationComponent *> mob_animation_component;
    gsl::not_null<MobHurtTimeComponent *> mob_hurt_time_component;
};

class Mob : public Actor {
public:
    Mob(Level &, EntityContext &);

    ~Mob() override = 0;
    ENDSTONE_HOOK virtual void knockback(Actor *, int, float, float, float, float, float);
    virtual void spawnAnim() = 0;
    virtual void setSprinting(bool) = 0;
    // virtual LevelSoundEvent getDeathSound() = 0;
    [[nodiscard]] virtual float getSpeed() const = 0;
    virtual void setSpeed(float) = 0;
    virtual void hurtEffects(ActorDamageSource const &, float, bool, bool) = 0;
    virtual void damageCarriedItemOnAttack(Actor &target, float) = 0;
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
    [[nodiscard]] virtual std::vector<ItemStack const *> getAllHandEquipment() const = 0;
    [[nodiscard]] virtual std::vector<ItemStack const *> getAllEquipment() const = 0;
    virtual void dropEquipmentOnDeath(ActorDamageSource const &, int) = 0;
    virtual void dropEquipmentOnDeath() = 0;
    virtual void clearVanishEnchantedItemsOnDeath() = 0;
    virtual void sendInventory(bool should_select_slot) = 0;
    [[nodiscard]] virtual float getDamageAfterEnchantReduction(ActorDamageSource const &, float) const = 0;
    virtual bool createAIGoals() = 0;
    virtual void onBorn(Actor &, Actor &) = 0;
    virtual bool setItemSlot(EquipmentSlot, ItemStack const &) = 0;
    virtual void setTransitioningSitting(bool) = 0;

protected:
    [[nodiscard]] virtual float _getWalkTargetValue(BlockPos const &) const = 0;

public:
    virtual bool canExistWhenDisallowMob() = 0;

protected:
    virtual std::unique_ptr<BodyControl> initBodyControl() = 0;
    virtual void newServerAiStep() = 0;
    // virtual void tickDeath() = 0;
    [[nodiscard]] virtual AABB _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &) const = 0;

public:
    static Mob *tryGetFromEntity(EntityContext &entity, bool include_removed = false);

    [[nodiscard]] bool isGliding() const;
    [[nodiscard]] bool isSprinting() const;
    void setYBodyRotation(float rotation);
    int getNoActionTime() const;
    void resetNoActionTime();
    bool checkForPostHitDamageImmunity(float damage_difference, const ActorDamageSource &source);

public:
    float hurt_dir;
    float o_tilt;
    float tilt;
    CompassSpriteCalculator compass_sprite_calc;
    CompassSpriteCalculator recovery_compass_sprite_calc;
    ClockSpriteCalculator clock_sprite_calc;

protected:
    float attack_anim_;
    int swing_time_;
    BuiltInMobComponents built_in_mob_components_;
    float movement_component_current_speed_;
    bool swinging_;
    bool surface_mob_;
    bool naturally_spawned_;
    bool wants_to_be_jockey_;

private:
    bool has_bound_origin_;
    std::optional<bool> actually_do_knockback_or_not_really_bad_hack_do_not_use_;
    MobSpawnMethod spawn_method_;
    bool create_ai_on_reload_;
    ActorUniqueID caravan_head_;
    ActorUniqueID caravan_tail_;
    float o_attack_anim_;
    BlockPos bound_origin_;
    ActorUniqueID target_captain_id_;
};
