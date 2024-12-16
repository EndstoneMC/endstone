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

#include <glm/glm.hpp>
#include <gsl/gsl>

#include "bedrock/bedrock.h"
#include "bedrock/core/math/color.h"
#include "bedrock/core/math/vec2.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/automatic_id.h"
#include "bedrock/entity/components/aabb_shape_component.h"
#include "bedrock/entity/components/actor_rotation_component.h"
#include "bedrock/entity/components/actor_walk_animation_component.h"
#include "bedrock/entity/components/attributes_component.h"
#include "bedrock/entity/components/state_vector_component.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/network/spatial_actor_network_data.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/server/commands/command_permission_level.h"
#include "bedrock/util/variant_parameter_list.h"
#include "bedrock/world/actor/actor_category.h"
#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/actor/actor_flags.h"
#include "bedrock/world/actor/actor_initialization_method.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_types.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/synched_actor_data_entity_wrapper.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/detail/actor/actor.h"

class Player;
class Level;

struct BuiltInActorComponents {
    gsl::not_null<StateVectorComponent *> state_vector_component;
    gsl::not_null<AABBShapeComponent *> aabb_shape_component;
    gsl::not_null<ActorRotationComponent *> actor_rotation_component;
    gsl::not_null<ActorWalkAnimationComponent *> walk_animation_component;
};

class Actor {
public:
    template <typename Component>
    [[nodiscard]] bool hasComponent() const
    {
        return entity_context_.hasComponent<Component>();
    }

    template <typename Component>
    Component *tryGetComponent()
    {
        return entity_context_.tryGetComponent<Component>();
    }

    template <typename Component>
    Component *tryGetComponent() const
    {
        return entity_context_.tryGetComponent<Component>();
    }

    template <typename Component>
    gsl::not_null<Component *> getPersistentComponent() const
    {
        return entity_context_.tryGetComponent<Component>();
    };

    virtual void outOfWorld() = 0;
    virtual void reloadHardcoded(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void reloadHardcodedClient(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void initializeComponents(ActorInitializationMethod, VariantParameterList const &) = 0;
    virtual void reloadComponents(ActorInitializationMethod, VariantParameterList const &) = 0;

protected:
    virtual void _serverInitItemStackIds() = 0;
    virtual void _doInitialMove() = 0;

public:
    virtual bool hasComponent(HashedString const &) = 0;
    virtual ~Actor() = 0;
    virtual void resetUserPos(bool) = 0;
    virtual ActorType getOwnerEntityType() = 0;
    ENDSTONE_HOOK virtual void remove();
    [[nodiscard]] virtual Vec3 getFiringPos() const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedHeadRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyYaw(float) const = 0;
    [[nodiscard]] virtual float getYawSpeedInDegreesPerSecond() const = 0;
    [[nodiscard]] virtual Vec3 getInterpolatedRidingOffset(float, int) const = 0;
    [[nodiscard]] virtual bool isFireImmune() const = 0;
    virtual void blockedByShield(ActorDamageSource const &, Actor &) = 0;
    virtual bool canDisableShield() = 0;
    ENDSTONE_HOOK virtual void teleportTo(Vec3 const &, bool, int, int, bool) = 0;
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
    [[nodiscard]] virtual bool isSleeping() const = 0;
    virtual void setSleeping(bool) = 0;
    virtual void setSneaking(bool) = 0;
    [[nodiscard]] virtual bool isBlocking() const = 0;
    [[nodiscard]] virtual bool isDamageBlocked(ActorDamageSource const &) const = 0;
    [[nodiscard]] virtual bool isAlive() const = 0;
    [[nodiscard]] virtual bool isOnFire() const = 0;
    [[nodiscard]] virtual bool isSurfaceMob() const = 0;
    [[nodiscard]] virtual bool isTargetable() const = 0;
    // virtual bool canAttack(Actor *, bool) const = 0;
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
    [[nodiscard]] virtual HashedString const &getActorRendererId() const = 0;
    virtual void despawn() = 0;
    virtual void setArmor(ArmorSlot, ItemStack const &) = 0;
    [[nodiscard]] virtual ArmorMaterialType getArmorMaterialTypeInSlot(ArmorSlot) const = 0;
    [[nodiscard]] virtual int getArmorTextureIndexInSlot(ArmorSlot) const = 0;
    [[nodiscard]] virtual float getArmorColorInSlot(ArmorSlot, int) const = 0;
    virtual void setEquippedSlot(EquipmentSlot, ItemStack const &) = 0;
    virtual void setCarriedItem(ItemStack const &) = 0;
    [[nodiscard]] virtual ItemStack const &getCarriedItem() const = 0;
    virtual void setOffhandSlot(ItemStack const &) = 0;
    [[nodiscard]] virtual ItemStack const &getEquippedTotem() const = 0;
    virtual bool consumeTotem() = 0;
    virtual bool load(CompoundTag const &, DataLoadHelper &) = 0;
    [[nodiscard]] virtual HashedString const &queryEntityRenderer() const = 0;
    [[nodiscard]] virtual ActorUniqueID getSourceUniqueID() const = 0;
    [[nodiscard]] virtual bool canFreeze() const = 0;
    [[nodiscard]] virtual AABB getLiquidAABB(MaterialType) const = 0;
    virtual void handleInsidePortal(BlockPos const &) = 0;
    [[nodiscard]] virtual bool canChangeDimensionsUsingPortal() const = 0;
    virtual void changeDimension(DimensionType) = 0;
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
    virtual void renderDebugServerState(Options const &) = 0;
    virtual void kill() = 0;
    virtual void die(ActorDamageSource const &) = 0;
    [[nodiscard]] virtual bool shouldDropDeathLoot() const = 0;
    virtual void applySnapshot(EntityContext const &, EntityContext const &) = 0;
    virtual float getNextStep(float) = 0;
    virtual void onPush(Actor &) = 0;
    [[nodiscard]] virtual std::optional<BlockPos> getLastDeathPos() const = 0;
    [[nodiscard]] virtual std::optional<DimensionType> getLastDeathDimension() const = 0;
    [[nodiscard]] virtual bool hasDiedBefore() const = 0;
    virtual void doEnterWaterSplashEffect() = 0;
    virtual void doExitWaterSplashEffect() = 0;
    virtual void doWaterSplashEffect() = 0;

protected:
    [[nodiscard]] virtual bool _shouldProvideFeedbackOnHandContainerItemSet(HandSlot, ItemStack const &) const = 0;
    [[nodiscard]] virtual bool _shouldProvideFeedbackOnArmorSet(ArmorSlot, ItemStack const &) const = 0;

public:
    virtual bool shouldTryMakeStepSound() = 0;

protected:
    virtual bool _hurt(ActorDamageSource const &, float, bool, bool) = 0;

public:
    virtual void readAdditionalSaveData(CompoundTag const &, DataLoadHelper &) = 0;
    virtual void addAdditionalSaveData(CompoundTag &) = 0;

protected:
    virtual void _playStepSound(BlockPos const &, Block const &) = 0;

public:
    [[nodiscard]] bool getStatusFlag(ActorFlags flags) const;
    [[nodiscard]] bool isType(ActorType type) const;
    [[nodiscard]] bool hasType(ActorType type) const;
    [[nodiscard]] bool isPlayer() const;
    [[nodiscard]] bool isRemoved() const;
    [[nodiscard]] bool isOnGround() const;
    [[nodiscard]] bool isInWater() const;
    [[nodiscard]] bool isInLava() const;
    [[nodiscard]] Dimension &getDimension() const;
    [[nodiscard]] Level &getLevel() const;
    [[nodiscard]] Vec3 const &getPosition() const;  // NOTE: this returns the eye position instead of feet position
    [[nodiscard]] Vec3 const &getPosPrev() const;
    void applyImpulse(Vec3 const &impulse);
    [[nodiscard]] Vec3 const &getPosDelta() const;
    void setPosDelta(const Vec3 &);
    [[nodiscard]] Vec2 const &getRotation() const;
    void setRotationWrapped(const Vec2 &);
    [[nodiscard]] AABB const &getAABB() const;
    [[nodiscard]] ActorRuntimeID getRuntimeID() const;
    [[nodiscard]] ActorUniqueID getOrCreateUniqueID() const;
    [[nodiscard]] Actor *getVehicle() const;
    [[nodiscard]] bool isRiding() const;
    [[nodiscard]] bool hasCategory(ActorCategory) const;
    [[nodiscard]] bool isJumping() const;
    [[nodiscard]] std::vector<std::string> getTags() const;
    bool addTag(const std::string &tag);
    bool removeTag(const std::string &tag);
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;

    [[nodiscard]] const AttributeInstance &getAttribute(const HashedString &name) const;  // Endstone
    [[nodiscard]] AttributeInstance &getMutableAttribute(const HashedString &name);       // Endstone

    static Actor *tryGetFromEntity(EntityContext const &, bool include_removed);

private:
    EntityContext entity_context_;  // +8
    VariantParameterList init_params_;
    std::string custom_init_event_name_;
    ActorInitializationMethod init_method_;
    bool force_init_method_to_spawn_on_reload_;

public:
    bool added;
    int send_rate_counter;
    ActorDefinitionGroup *definitions;
    std::unique_ptr<ActorDefinitionDescriptor> current_description;
    std::shared_ptr<RopeSystem> leash_rope_system;
    std::string alias;
    std::optional<glm::mat4x4> previous_render_transform;
    int last_hurt_by_player_time;
    std::map<HashedString, std::vector<std::vector<glm::mat4x4>>> previous_bone_matrices;
    SynchedActorDataEntityWrapper entity_data;
    std::unique_ptr<SpatialActorNetworkData> network_data;
    Vec3 sent_delta;
    float scale;
    float scale_prev;
    bool ignore_lighting;
    bool filter_lighting;
    float step_sound_volume;
    float step_sound_pitch;
    AABB *last_hit_bb;
    HashType64 name_tag_hash;
    float shadow_offset;
    float pushthrough;
    int tick_count;
    int invulnerable_time;
    int last_health;
    bool hurt_marked;
    bool was_hurt_last_frame;
    bool invulnerable;
    int flame_tex_frame_index;
    float flame_frame_increment_time;
    bool always_fire_immune;
    bool inherit_rotation_when_riding;
    bool forced_loading;
    bool force_send_motion_packet;
    bool highlighted_this_frame;
    bool initialized;
    bool processed_on_chunk_discard;
    float sound_volume;
    int shake_time;
    ActorUniqueID legacy_unique_id;

private:
    WeakRef<Dimension> dimension_;                // +456
    Level *level_;                                // +472
    HashedString actor_renderer_id_;              //
    ActorCategory categories_;                    //
    BuiltInActorComponents built_in_components_;  //
    // ...

    endstone::detail::EndstoneActor &getEndstoneActor0() const;

public:
    template <typename T = endstone::detail::EndstoneActor>
    [[nodiscard]] T &getEndstoneActor() const
    {
        return static_cast<T &>(getEndstoneActor0());
    }
};
