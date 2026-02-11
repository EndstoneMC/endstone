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
#include "bedrock/input/input_mode.h"
#include "bedrock/network/spatial_actor_network_data.h"
#include "bedrock/server/commands/command_permission_level.h"
#include "bedrock/shared_types/legacy/level_sound_event.h"
#include "bedrock/util/molang_variable_map.h"
#include "bedrock/util/variant_parameter_list.h"
#include "bedrock/world/actor/actor_category.h"
#include "bedrock/world/actor/actor_damage_source.h"
#include "bedrock/world/actor/actor_definition_identifier.h"
#include "bedrock/world/actor/actor_definition_ptr.h"
#include "bedrock/world/actor/actor_flags.h"
#include "bedrock/world/actor/actor_initialization_method.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/actor/actor_swing_source.h"
#include "bedrock/world/actor/actor_terrain_interlock_data.h"
#include "bedrock/world/actor/actor_types.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/synched_actor_data.h"
#include "bedrock/world/effect/mob_effect_instance.h"
#include "bedrock/world/gamemode/interaction_result.h"
#include "bedrock/world/item/equipment_slot.h"
#include "bedrock/world/item/item_helper.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/actor/actor.h"

class Player;
class Level;

enum class SpawnRuleEnum : int {
    Undefined = -1,
    NoSpawnRules = 0,
    HasSpawnRules = 1,
};

struct BuiltInActorComponents {
    gsl::not_null<StateVectorComponent *> state_vector_component;
    gsl::not_null<AABBShapeComponent *> aabb_shape_component;
    gsl::not_null<ActorRotationComponent *> actor_rotation_component;
    gsl::not_null<ActorWalkAnimationComponent *> walk_animation_component;
};

class Actor {
public:
    template <typename Component>
    void addOrRemoveComponent(const bool should_add)
    {
        if (should_add) {
            entity_context_.getOrAddComponent<Component>();
        }
        else {
            entity_context_.removeComponent<Component>();
        }
    }

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
    gsl::not_null<Component *> getPersistentComponent()
    {
        return entity_context_.tryGetComponent<Component>();
    }

    template <typename Component>
    gsl::not_null<const Component *> getPersistentComponent() const
    {
        return entity_context_.tryGetComponent<Component>();
    }

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
    virtual void remove() = 0;
    [[nodiscard]] virtual Vec3 getFiringPos() const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedHeadRot(float) const = 0;
    [[nodiscard]] virtual float getInterpolatedBodyYaw(float) const = 0;
    [[nodiscard]] virtual float getYawSpeedInDegreesPerSecond() const = 0;
    [[nodiscard]] virtual Vec3 getInterpolatedRidingOffset(float, int) const = 0;
    [[nodiscard]] virtual bool isFireImmune() const = 0;
    virtual void blockedByShield(ActorDamageSource const &, Actor &) = 0;
    virtual bool canDisableShield() = 0;
    ENDSTONE_HOOK virtual void teleportTo(Vec3 const &pos, bool should_stop_riding, int cause, int source_entity_type,
                                          bool keep_velocity) = 0;
    virtual Vec3 lerpMotion(Vec3 const &) = 0;
    virtual std::unique_ptr<Packet> tryCreateAddActorPacket() = 0;
    virtual void normalTick() = 0;
    virtual void baseTick() = 0;
    virtual void passengerTick() = 0;
    virtual bool startRiding(Actor &) = 0;
    virtual void addPassenger(Actor &) = 0;
    [[nodiscard]] virtual std::string getExitTip(std::string const &, InputMode, NewInteractionModel) const = 0;
    [[nodiscard]] virtual std::string getEntityLocNameString() const = 0;
    [[nodiscard]] virtual bool isInvisible() const = 0;
    [[nodiscard]] virtual bool canShowNameTag() const = 0;
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
    virtual void playAmbientSound() = 0;
    [[nodiscard]] virtual LevelSoundEvent getAmbientSound() const = 0;
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
    virtual void sendMotionPacketIfNeeded() = 0;
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
    virtual bool swing(ActorSwingSource swing_source) = 0;
    virtual void useItem(ItemStackBase &, ItemUseMethod, bool) = 0;
    virtual void getDebugText(std::vector<std::string> &) = 0;
    [[nodiscard]] virtual float getMapDecorationRotation() const = 0;
    [[nodiscard]] virtual float getPassengerYRotation(Actor const &) const = 0;
    virtual bool add(ItemStack &) = 0;
    virtual bool drop(ItemStack const &, bool) = 0;
    virtual InteractionResult getInteraction(Player &, ActorInteraction &, Vec3 const &) = 0;
    [[nodiscard]] virtual bool canDestroyBlock(Block const &) const = 0;
    virtual void setAuxValue(int) = 0;
    virtual void renderDebugServerState(const IOptionsReader &options) = 0;
    virtual void kill() = 0;
    virtual void die(ActorDamageSource const &) = 0;
    [[nodiscard]] virtual bool shouldDropDeathLoot() const = 0;
    virtual void applySnapshot(EntityContext const &, EntityContext const &) = 0;
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
    virtual bool _hurt(ActorDamageSource const &, float, bool, bool) = 0;

public:
    virtual void readAdditionalSaveData(CompoundTag const &, DataLoadHelper &) = 0;
    virtual void addAdditionalSaveData(CompoundTag &) = 0;

public:
    Actor(ILevel &, EntityContext &);
    [[nodiscard]] bool getStatusFlag(ActorFlags) const;
    void setStatusFlag(ActorFlags, bool);
    [[nodiscard]] bool isType(ActorType type) const;
    [[nodiscard]] bool hasType(ActorType types) const;
    [[nodiscard]] ActorType getEntityTypeId() const;
    [[nodiscard]] const ActorDefinitionIdentifier &getActorIdentifier() const;
    BuiltInActorComponents &getBuiltInActorComponents();
    [[nodiscard]] const BuiltInActorComponents &getBuiltInActorComponents() const;
    [[nodiscard]] bool isSneaking() const;
    [[nodiscard]] bool isPlayer() const;
    [[nodiscard]] bool isRemoved() const;
    [[nodiscard]] bool isOnGround() const;
    [[nodiscard]] bool wasOnGround() const;
    [[nodiscard]] bool isInWater() const;
    [[nodiscard]] bool isInLava() const;
    [[nodiscard]] bool isClientSide() const;
    BlockSource &getDimensionBlockSource() const;
    [[nodiscard]] const BlockSource &getDimensionBlockSourceConst() const;
    [[nodiscard]] Dimension &getDimension() const;
    [[nodiscard]] DimensionType getDimensionId() const;
    [[nodiscard]] Level &getLevel();
    [[nodiscard]] const Level &getLevel() const;
    void setAABB(const AABB &bb);
    [[nodiscard]] const AABB &getAABB() const;
    [[nodiscard]] const Vec2 &getAABBDim() const;
    [[nodiscard]] Vec3 const &getPosition() const;  // NOTE: this returns the eye position instead of feet position
    [[nodiscard]] Vec3 const &getPosPrev() const;
    void setPos(const Vec3 &);
    void applyImpulse(Vec3 const &impulse);
    [[nodiscard]] Vec3 const &getPosDelta() const;
    void setPosDelta(const Vec3 &);
    [[nodiscard]] Vec2 const &getRotation() const;
    [[nodiscard]] const Vec2 &getRotationPrev() const;
    void setRotationWrapped(const Vec2 &);
    [[nodiscard]] ActorRuntimeID getRuntimeID() const;
    [[nodiscard]] ActorUniqueID getOrCreateUniqueID() const;
    [[nodiscard]] Actor *getVehicle() const;
    [[nodiscard]] bool isRiding() const;
    void stopRiding(bool exit_from_passenger, bool actor_is_being_destroyed, bool switching_vehicles,
                    bool is_being_teleported);
    [[nodiscard]] bool hasPassenger() const;
    void removeAllPassengers(bool being_destroyed);
    [[nodiscard]] bool hasCategory(ActorCategory categories) const;
    [[nodiscard]] bool isJumping() const;
    [[nodiscard]] std::vector<std::string> getTags() const;
    bool addTag(const std::string &tag);
    bool removeTag(const std::string &tag);
    [[nodiscard]] int getHealth() const;
    [[nodiscard]] int getMaxHealth() const;
    void setNameTagVisible(bool);
    [[nodiscard]] const std::string &getNameTag() const;
    void setNameTag(const std::string &);
    [[nodiscard]] const std::string &getScoreTag() const;
    void setScoreTag(const std::string &);
    [[nodiscard]] const AttributeInstance *getAttribute(const HashedString &name) const;      // Endstone
    [[nodiscard]] MutableAttributeWithContext getMutableAttribute(const HashedString &name);  // Endstone
    [[nodiscard]] float getFallDistance() const;
    void setFallDistance(float);
    [[nodiscard]] bool isDead() const;
    EntityContext &getEntity();
    [[nodiscard]] const EntityContext &getEntity() const;
    [[nodiscard]] WeakRef<EntityContext> getWeakEntity() const;
    [[nodiscard]] const ItemStack &getOffhandSlot() const;
    [[nodiscard]] const ItemStack &getArmor(ArmorSlot) const;
    [[nodiscard]] bool isCreative() const;
    [[nodiscard]] bool isAdventure() const;
    [[nodiscard]] bool isSurvival() const;
    [[nodiscard]] bool isSpectator() const;
    void queueBBUpdateFromValue(const Vec2 &);
    void queueBBUpdateFromDefinition();
    void setChainedDamageEffects(bool);
    [[nodiscard]] bool getChainedDamageEffects() const;
    Mob *getLastHurtByMob();
    [[nodiscard]] ActorUniqueID getLastHurtByMobID() const;
    void setLastHurtByMob(Mob *);
    Player *getLastHurtByPlayer();
    [[nodiscard]] ActorUniqueID getLastHurtByPlayerID() const;
    void setLastHurtByPlayer(Player *);
    Mob *getLastHurtMob();
    void setLastHurtMob(const Mob &target);
    int getLastHurtMobTimestamp();
    int getLastHurtByMobTime();
    int getLastHurtByMobTimestamp();
    [[nodiscard]] bool hasBeenHurtByMobInLastTicks(int) const;
    [[nodiscard]] float getLastHurtDamage() const;
    [[nodiscard]] ActorDamageCause getLastHurtCause() const;
    [[nodiscard]] std::uint64_t getLastHurtTimestamp() const;
    [[nodiscard]] bool isUseNewTradeScreen() const;
    [[nodiscard]] bool canSeeDaylight() const;
    [[nodiscard]] const Block *getInsideBlock() const;
    void setInsideBlock(const Block *);
    [[nodiscard]] const BlockPos &getInsideBlockPos() const;
    void setInsideBlockPos(const BlockPos &);

    static Actor *tryGetFromEntity(EntityContext const &, bool include_removed = false);
    static Actor *tryGetFromEntity(StackRefResult<EntityContext>, bool include_removed = false);

    void setLastHurtDamage(float damage);  // Endstone

protected:
    void _setHeightOffset(float offset);
    void _moveHitboxTo(const Vec3 &position);
    void _setDimensionTransitionComponent(DimensionType from_id, DimensionType to_id, int portal_cooldown);

private:
    mutable EntityContext entity_context_;  // +8
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
    std::map<HashedString, std::vector<std::vector<glm::highp_fmat4x4>>> previous_bone_matrices;
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
    float push_through;
    int tick_count;
    int invulnerable_time;
    int last_health;
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
    WeakRef<Dimension> dimension_;
    ILevel *level_;
    HashedString actor_renderer_id_;
    ActorCategory categories_;
    BuiltInActorComponents built_in_components_;

protected:
    HashedString actor_renderer_id_that_animation_component_was_initialized_with_;
    bool changed_;
    bool removed_;
    bool moved_to_limbo_;
    bool moved_to_unloaded_chunk_;
    bool blocks_building_;
    std::shared_ptr<AnimationComponent> animation_component_;
    std::shared_ptr<AnimationComponent> ui_animation_component_;
    ActorUniqueID target_id_;
    ActorUniqueID in_love_partner_;
    std::unique_ptr<CompoundTag> persisting_trade_offers_;
    int persisting_trade_riches_;
    bool persisting_trade_;
    bool effects_dirty_;
    bool loot_dropped_;
    bool loaded_from_nbt_this_frame_;

public:
protected:
    Color hurt_color_;
    std::unique_ptr<ActorDefinitionDiffList> definition_list_;
    std::unique_ptr<CompoundTag> loaded_actor_property_bag_;
    ActorDefinitionPtr actor_definition_ptr_;
    std::string filtered_name_tag_;
    ActorTerrainInterlockData terrain_interlock_data_;
    bool was_hurt_this_tick_;
    bool was_hurt_last_tick_;
    ActorUniqueID last_hurt_mob_id_;
    ActorUniqueID last_hurt_by_mob_id_;
    ActorUniqueID last_hurt_by_player_id_;
    uint64_t last_hurt_timestamp_;
    ActorDamageCause last_hurt_cause_;
    float last_hurt_;
    int last_hurt_mob_timestamp_;
    int last_hurt_by_mob_time_;
    int last_hurt_by_mob_timestamp_;
    bool is_predictable_projectile_;
    bool is_rendering_in_ui_;
    bool is_on_screen_;
    bool update_bones_and_effects_;
    bool update_effects_;

private:
    bool can_pickup_items_;
    bool has_set_can_pickup_items_;
    bool chained_damage_effects_;
    // bool was_in_wall_last_tick_;
    // int ticks_in_wall_;
    int affected_by_water_bottle_ticks_to_effect_;
    SpawnRuleEnum spawn_rules_enum_;
    std::unique_ptr<ActionQueue> action_queue_;
    MolangVariableMap molang_variables_;
    ActorUniqueID fishing_hook_id_;

public:
    template <typename T = endstone::core::EndstoneActor>
    [[nodiscard]] T &getEndstoneActor() const
    {
        return static_cast<T &>(getEndstoneActor0());
    }

    template <typename T = endstone::core::EndstoneActor>
    [[nodiscard]] std::shared_ptr<T> getEndstoneActorPtr() const
    {
        return std::static_pointer_cast<T>(getEndstoneActorPtr0());
    }

private:
    endstone::Actor &getEndstoneActor0() const;
    std::shared_ptr<endstone::Actor> getEndstoneActorPtr0() const;
};
