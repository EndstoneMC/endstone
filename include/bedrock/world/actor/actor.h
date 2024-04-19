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

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock/automatic_id.h"
#include "bedrock/bedrock.h"
#include "bedrock/command/command_permission_level.h"
#include "bedrock/forward.h"
#include "bedrock/mce.h"
#include "bedrock/memory.h"
#include "bedrock/world/actor/actor_initialization_method.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/registry/entity_context.h"
#include "bedrock/world/actor/registry/entity_registry.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/math/vec3.h"
#include "endstone/detail/actor/actor.h"

class Player;
class Level;

class Actor {
public:
    virtual bool getStatusFlag(ActorFlags flags) = 0;
    virtual void setStatusFlag(ActorFlags flags, bool value) = 0;
#ifdef _WIN32
    virtual bool hasComponent(HashedString const &) = 0;
#endif
    virtual void outOfWorld() = 0;
    virtual void reloadHardcoded(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void reloadHardcodedClient(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void initializeComponents(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void reloadComponents(ActorInitializationMethod, VariantParameterList const &) = 0;

protected:
    virtual void _serverInitItemStackIds() = 0;
    virtual void _doInitialMove() = 0;

public:
#ifdef __linux__
    virtual bool hasComponent(HashedString const &) = 0;
#endif
    virtual ~Actor() = 0;
    virtual void resetUserPos(bool) = 0;
    virtual ActorType getOwnerEntityType() = 0;
    BEDROCK_API virtual void remove();
    virtual bool isRuntimePredictedMovementEnabled() = 0;
    [[nodiscard]] virtual Vec3 getFiringPos() const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedHeadRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyYaw(float) const = 0;
    [[nodiscard]] virtual float getYawSpeedInDegreesPerSecond() const = 0;
    [[nodiscard]] virtual Vec3 getInterpolatedRidingOffset(float, int) const = 0;
    virtual void resetInterpolated() = 0;
    [[nodiscard]] virtual bool isFireImmune() const = 0;
    virtual void blockedByShield(ActorDamageSource const &, Actor &) = 0;
    virtual bool canDisableShield() = 0;
    virtual void teleportTo(Vec3 const &, bool, int, int, bool) = 0;
    virtual Vec3 lerpMotion(Vec3 const &) = 0;
    virtual std::unique_ptr<AddActorBasePacket> tryCreateAddActorPacket() = 0;
    virtual void normalTick() = 0;
    virtual void baseTick() = 0;
    virtual void passengerTick() = 0;
    virtual bool startRiding(Actor &) = 0;
    virtual void addPassenger(Actor &) = 0;
    [[nodiscard]] virtual std::string getExitTip(std::string const &, InputMode, NewInteractionModel) const = 0;
    [[nodiscard]] virtual std::string getEntityLocNameString() const = 0;
    [[nodiscard]] virtual bool isInWall() const = 0;
    [[nodiscard]] virtual bool isInvisible() const = 0;
    [[nodiscard]] virtual bool canShowNameTag() const = 0;
    [[nodiscard]] virtual std::string getFormattedNameTag() const = 0;
    [[nodiscard]] virtual mce::Color getNameTagTextColor() const = 0;
    [[nodiscard]] virtual float getShadowRadius() const = 0;
    [[nodiscard]] virtual Vec3 getHeadLookVector(float) const = 0;
    [[nodiscard]] virtual bool canInteractWithOtherEntitiesInGame() const = 0;
    virtual float getBrightness(float, IConstBlockSource const &) = 0;
    virtual void playerTouch(Player &) = 0;
    [[nodiscard]] virtual bool isImmobile() const = 0;
    virtual bool isSilentObserver() = 0;
    virtual bool isPickable() = 0;
    [[nodiscard]] virtual bool isSleeping() const = 0;
    virtual void setSleeping(bool) = 0;
    virtual void setSneaking(bool) = 0;
    [[nodiscard]] virtual bool isBlocking() const = 0;
    [[nodiscard]] virtual bool isDamageBlocked(ActorDamageSource const &) const = 0;
    [[nodiscard]] virtual bool isAlive() const = 0;
    [[nodiscard]] virtual bool isOnFire() const = 0;
    [[nodiscard]] virtual bool isSurfaceMob() const = 0;
    [[nodiscard]] virtual bool isTargetable() const = 0;
    [[nodiscard]] virtual bool isLocalPlayer() const = 0;
    [[nodiscard]] virtual bool isPlayer() const = 0;
    virtual bool canAttack(Actor *, bool) const = 0;
    virtual void setTarget(Actor *) = 0;
    virtual bool isValidTarget(Actor *) const = 0;
    virtual bool attack(Actor &, ActorDamageCause const &) = 0;
    virtual void performRangedAttack(Actor &, float) = 0;
    virtual void setOwner(ActorUniqueID) = 0;
    virtual void setSitting(bool) = 0;
    virtual void onTame() = 0;
    virtual void onFailedTame() = 0;
    virtual void setStanding(bool) = 0;
    [[nodiscard]] virtual bool canPowerJump() const = 0;
    [[nodiscard]] virtual bool isEnchanted() const = 0;
    [[nodiscard]] virtual bool shouldRender() const = 0;
    virtual void playAmbientSound() = 0;
    [[nodiscard]] virtual Puv::Legacy::LevelSoundEvent getAmbientSound() const = 0;
    [[nodiscard]] virtual bool isInvulnerableTo(ActorDamageSource const &) const = 0;
    [[nodiscard]] virtual ActorDamageCause getBlockDamageCause(Block const &) const = 0;
    virtual bool doFireHurt(int) = 0;
    virtual void onLightningHit() = 0;
    virtual void feed(int) = 0;
    virtual void handleEntityEvent(ActorEvent, int) = 0;
    virtual float getPickRadius() = 0;
    [[nodiscard]] virtual HashedString const &getActorRendererId() const = 0;
    virtual void despawn() = 0;
    virtual void setArmor(ArmorSlot, ItemStack const &) = 0;
    [[nodiscard]] virtual ArmorMaterialType getArmorMaterialTypeInSlot(ArmorSlot) const = 0;
    [[nodiscard]] virtual ArmorTextureType getArmorMaterialTextureTypeInSlot(ArmorSlot) const = 0;
    [[nodiscard]] virtual float getArmorColorInSlot(ArmorSlot, int) const = 0;
    virtual void setEquippedSlot(EquipmentSlot, ItemStack const &) = 0;
    virtual void setCarriedItem(ItemStack const &) = 0;
    [[nodiscard]] virtual ItemStack const &getCarriedItem() const = 0;
    virtual void setOffhandSlot(ItemStack const &) = 0;
    [[nodiscard]] virtual ItemStack const &getEquippedTotem() const = 0;
    virtual bool consumeTotem() = 0;
    virtual bool save(CompoundTag &) const = 0;
    virtual bool load(CompoundTag const &, DataLoadHelper &) = 0;
    [[nodiscard]] virtual HashedString const &queryEntityRenderer() const = 0;
    [[nodiscard]] virtual ActorUniqueID getSourceUniqueID() const = 0;
    [[nodiscard]] virtual bool canFreeze() const = 0;
    [[nodiscard]] virtual AABB getLiquidAABB(MaterialType) const = 0;
    virtual void handleInsidePortal(BlockPos const &) = 0;
    [[nodiscard]] virtual bool canChangeDimensionsUsingPortal() const = 0;
    virtual void changeDimension(AutomaticID<Dimension, int>) = 0;
    virtual void changeDimension(ChangeDimensionPacket const &) = 0;
    [[nodiscard]] virtual ActorUniqueID getControllingPlayer() const = 0;
    virtual float causeFallDamageToActor(float, float, ActorDamageSource) = 0;
    virtual void onSyncedDataUpdate(int) = 0;
    virtual bool canAddPassenger(Actor &) const = 0;
    [[nodiscard]] virtual bool canPickupItem(ItemStack const &) const = 0;
    [[nodiscard]] virtual bool canBePulledIntoVehicle() const = 0;
    [[nodiscard]] virtual bool inCaravan() const = 0;
    virtual void sendMotionPacketIfNeeded(PlayerMovementSettings const &) = 0;
    [[nodiscard]] virtual bool canSynchronizeNewEntity() const = 0;
    virtual void startSwimming() = 0;
    virtual void stopSwimming() = 0;
    virtual void buildDebugInfo(std::string &) const = 0;
    [[nodiscard]] virtual CommandPermissionLevel getCommandPermissionLevel() const = 0;
    [[nodiscard]] virtual int getDeathTime() const = 0;
    [[nodiscard]] virtual bool canBeAffected(std::uint32_t) const = 0;
    [[nodiscard]] virtual bool canBeAffectedByArrow(MobEffectInstance const &) const = 0;
    virtual void onEffectRemoved(MobEffectInstance &) = 0;
    [[nodiscard]] virtual bool canObstructSpawningAndBlockPlacement() const = 0;
    virtual AnimationComponent &getAnimationComponent() = 0;
    virtual void openContainerComponent(Player &) = 0;
    virtual void swing() = 0;
    virtual void useItem(ItemStackBase &, ItemUseMethod, bool) = 0;
    virtual void getDebugText(std::vector<std::string> &) = 0;
    [[nodiscard]] virtual float getMapDecorationRotation() const = 0;
    [[nodiscard]] virtual float getPassengerYRotation(Actor const &) const = 0;
    virtual bool add(ItemStack &) = 0;
    virtual bool drop(ItemStack const &, bool) = 0;
    virtual bool getInteraction(Player &, ActorInteraction &, Vec3 const &) = 0;
    [[nodiscard]] virtual bool canDestroyBlock(Block const &) const = 0;
    virtual void setAuxValue(int) = 0;
    virtual void startSpinAttack() = 0;
    virtual void stopSpinAttack() = 0;
    virtual void renderDebugServerState(Options const &) = 0;
    virtual void kill() = 0;
    virtual void die(ActorDamageSource const &) = 0;
    [[nodiscard]] virtual bool shouldDropDeathLoot() const = 0;
    virtual void applySnapshot(EntityContext const &, EntityContext const &) = 0;
    virtual float getNextStep(float) = 0;
    virtual void onPush(Actor &) = 0;
    [[nodiscard]] virtual std::optional<BlockPos> getLastDeathPos() const = 0;
    [[nodiscard]] virtual std::optional<AutomaticID<Dimension, int>> getLastDeathDimension() const = 0;
    [[nodiscard]] virtual bool hasDiedBefore() const = 0;
    virtual void doEnterWaterSplashEffect() = 0;
    virtual void doExitWaterSplashEffect() = 0;
    virtual void doWaterSplashEffect() = 0;

protected:
    [[nodiscard]] virtual bool _shouldProvideFeedbackOnHandContainerItemSet(HandSlot, ItemStack const &) const = 0;
    [[nodiscard]] virtual bool _shouldProvideFeedbackOnArmorSet(ArmorSlot, ItemStack const &) const = 0;

public:
    virtual void updateEntitySpecificMolangVariables(RenderParams &) = 0;
    virtual bool shouldTryMakeStepSound() = 0;

protected:
    virtual bool _hurt(ActorDamageSource const &, float, bool, bool) = 0;

public:
    virtual void readAdditionalSaveData(CompoundTag const &, DataLoadHelper &) = 0;
    virtual void addAdditionalSaveData(CompoundTag &) = 0;

protected:
    virtual void _playStepSound(BlockPos const &, Block const &) = 0;
    virtual void _doAutoAttackOnTouch(Actor &) = 0;

public:
    template <typename Component>
    Component *tryGetComponent()
    {
        return context_.tryGetComponent<Component>();
    }

    template <typename Component>
    Component *tryGetComponent() const
    {
        return context_.tryGetComponent<Component>();
    }

    BEDROCK_API void setDimension(WeakRef<Dimension>);

    [[nodiscard]] bool isRemoved() const;
    [[nodiscard]] Dimension &getDimension() const;
    [[nodiscard]] Level &getLevel() const;
    [[nodiscard]] Vec3 const &getPosition() const;
    [[nodiscard]] ActorRuntimeID getRuntimeID() const;

    static Actor *tryGetFromEntity(EntityContext const &, bool include_removed);

protected:
    EntityContext context_;                            // +8
    ActorInitializationMethod initialization_method_;  // +32
    std::string unknown2_;                             // +40
    std::array<char[16], 10> unknown3_;                // +72
    std::int16_t unknown4_;                            // +232
    ActorDefinitionGroup *actor_definitions_;          // +240
    char pad_[336];                                    // +248 TODO: figure out the structure
    WeakRef<Dimension> dimension_;                     // +584
    Level *level_;                                     // +600
    char pad2_[56];                                    // +608
    Vec3 position_;                                    // +664

public:
    [[nodiscard]] endstone::detail::EndstoneActor &getEndstoneActor() const;
};
