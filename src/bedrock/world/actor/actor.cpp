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

#include "bedrock/world/actor/actor.h"

#include "bedrock/entity/components/actor_definition_identifier_component.h"
#include "bedrock/entity/components/actor_equipment_component.h"
#include "bedrock/entity/components/actor_game_type_component.h"
#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/entity/components/actor_type_flag_component.h"
#include "bedrock/entity/components/actor_unique_id_component.h"
#include "bedrock/entity/components/autonomous_actor_component.h"
#include "bedrock/entity/components/dimension_transition_component.h"
#include "bedrock/entity/components/dimension_type_component.h"
#include "bedrock/entity/components/fall_distance_component.h"
#include "bedrock/entity/components/global_actor_component.h"
#include "bedrock/entity/components/is_dead_flag_component.h"
#include "bedrock/entity/components/passenger_component.h"
#include "bedrock/entity/components/player_component.h"
#include "bedrock/entity/components/portal_cooldown_duration_component.h"
#include "bedrock/entity/components/runtime_id_component.h"
#include "bedrock/entity/components/should_update_bounding_box_request_component.h"
#include "bedrock/entity/components/tags_component.h"
#include "bedrock/entity/systems/tag_system.h"
#include "bedrock/entity/utilities/rotation_utility.h"
#include "bedrock/entity/utilities/synched_actor_data_access.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/symbol.h"
#include "bedrock/world/actor/actor_collision.h"
#include "bedrock/world/actor/actor_definition_diff_list.h"
#include "bedrock/world/actor/actor_environment.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/mob_jump.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/actor/provider/actor_offset.h"
#include "bedrock/world/actor/provider/actor_riding.h"
#include "bedrock/world/level/block/states/vanilla_block_states.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"

bool Actor::getStatusFlag(ActorFlags flag) const
{
    return SynchedActorDataAccess::getActorFlag(entity_context_, flag);
}

void Actor::setStatusFlag(ActorFlags flag, bool value)
{
    SynchedActorDataAccess::setActorFlag(entity_context_, flag, value);
}

bool Actor::isType(ActorType type) const
{
    return type == getEntityTypeId();
}

bool Actor::hasType(ActorType types) const
{
    if (static_cast<std::underlying_type_t<ActorType>>(types)) {
        return types == getEntityTypeId();
    }
    return (types & getEntityTypeId()) == types;
}

ActorType Actor::getEntityTypeId() const
{
    return getPersistentComponent<ActorTypeComponent>()->type;
}

const ActorDefinitionIdentifier &Actor::getActorIdentifier() const
{
    static ActorDefinitionIdentifier empty;
    if (auto *component = tryGetComponent<ActorDefinitionIdentifierComponent>(); component) {
        return component->identifier;
    }
    return empty;
}

const BuiltInActorComponents &Actor::getBuiltInActorComponents() const
{
    return built_in_components_;
}

bool Actor::isSneaking() const
{
    return getStatusFlag(ActorFlags::SNEAKING);
}

bool Actor::isPlayer() const
{
    return hasComponent<PlayerComponent>();
}

bool Actor::isRemoved() const
{
    return removed_;
}

bool Actor::isOnGround() const
{
    return ActorCollision::isOnGround(entity_context_);
}

bool Actor::wasOnGround() const
{
    return ActorCollision::wasOnGround(entity_context_);
}

bool Actor::isInWater() const
{
    return ActorEnvironment::getIsInWater(entity_context_);
}

bool Actor::isInLava() const
{
    return ActorEnvironment::getIsInLava(entity_context_);
}
bool Actor::isClientSide() const
{
    if (!level_) {
        return true;
    }
    return level_->isClientSide();
}

BlockSource &Actor::getDimensionBlockSource() const
{
    return dimension_.unwrap()->getBlockSourceFromMainChunkSource();
}

Dimension &Actor::getDimension() const
{
    return *dimension_.unwrap();
}

DimensionType Actor::getDimensionId() const
{
    return getPersistentComponent<DimensionTypeComponent>()->type;
}

Level &Actor::getLevel()
{
    return *static_cast<Level *>(level_);
}

const Level &Actor::getLevel() const
{
    return *static_cast<Level *>(level_);
}

void Actor::setAABB(const AABB &bb)
{
    built_in_components_.aabb_shape_component->aabb = bb;
}

Vec3 const &Actor::getPosition() const
{
    return built_in_components_.state_vector_component->pos;
}

Vec3 const &Actor::getPosPrev() const
{
    return built_in_components_.state_vector_component->pos_prev;
}

// void Actor::setPos(const Vec3 &position)
// {
//     ActorSetPosSystem::setPosition(getEntity(), position, level_ == nullptr || level_->isClientSide());
// }

void Actor::applyImpulse(Vec3 const &impulse)
{
    built_in_components_.state_vector_component->pos_delta += impulse;
}

Vec3 const &Actor::getPosDelta() const
{
    return built_in_components_.state_vector_component->pos_delta;
}

void Actor::setPosDelta(const Vec3 &pos_delta)
{
    built_in_components_.state_vector_component->pos_delta = pos_delta;
}

Vec2 const &Actor::getRotation() const
{
    return built_in_components_.actor_rotation_component->rotation_degree;
}

const Vec2 &Actor::getRotationPrev() const
{
    return built_in_components_.actor_rotation_component->rotation_degree_previous;
}

void Actor::setRotationWrapped(const Vec2 &rot)
{
    RotationUtility::setRot(rot, built_in_components_.actor_rotation_component->rotation_degree,
                            built_in_components_.actor_rotation_component->rotation_degree_previous);
}

AABB const &Actor::getAABB() const
{
    return built_in_components_.aabb_shape_component->aabb;
}

const Vec2 &Actor::getAABBDim() const
{
    return built_in_components_.aabb_shape_component->bb_dim;
}

ActorRuntimeID Actor::getRuntimeID() const
{
    return tryGetComponent<RuntimeIDComponent>()->runtime_id;
}

ActorUniqueID Actor::getOrCreateUniqueID() const
{
    auto component = getPersistentComponent<ActorUniqueIDComponent const>();
    if (component->unique_id.raw_id != -1) {
        return component->unique_id;
    }
    auto unique_id = level_->getNewUniqueID();
    return entity_context_.getOrAddComponent<ActorUniqueIDComponent>(unique_id).unique_id;
}

Actor *Actor::getVehicle() const
{
    const auto *component = tryGetComponent<PassengerComponent>();
    if (component && !component->vehicle.entity.isNull()) {
        return level_->fetchEntity(component->vehicle.actor_id, false);
    }
    return nullptr;
}

bool Actor::isRiding() const
{
    return getVehicle() != nullptr;
}

// void Actor::stopRiding(bool exit_from_passenger, bool actor_is_being_destroyed, bool switching_vehicles,
//                        bool is_being_teleported)
// {
//     BEDROCK_CALL(&Actor::stopRiding, this, exit_from_passenger, actor_is_being_destroyed, switching_vehicles,
//                  is_being_teleported);
// }

// bool Actor::hasPassenger() const
// {
//     return !ActorRiding::getPassengers(getEntity()).empty();
// }

bool Actor::hasCategory(ActorCategory categories) const
{
    return (categories & categories_) == categories;
}

float Actor::getLastHurtDamage() const
{
    return last_hurt_;
}

Actor *Actor::tryGetFromEntity(EntityContext const &entity, bool include_removed)
{
    auto *component = entity.tryGetComponent<ActorOwnerComponent>();
    if (!component) {
        return nullptr;
    }

    auto &actor = component->getActor();
    if (!actor.isRemoved() || include_removed) {
        return &actor;
    }
    return nullptr;
}

Actor *Actor::tryGetFromEntity(StackRefResult<EntityContext> entity, bool include_removed)
{
    if (!entity) {
        return nullptr;
    }
    return tryGetFromEntity(*entity, include_removed);
}

void Actor::setLastHurtDamage(float damage)
{
    last_hurt_ = damage;
}

void Actor::_setDimensionTransitionComponent(DimensionType from_id, DimensionType to_id, int portal_cooldown)
{
    auto axis = PortalAxis::Unknown;
    if ((from_id == VanillaDimensions::Nether || to_id == VanillaDimensions::Nether) &&
        from_id != VanillaDimensions::TheEnd) {
        const auto component = getPersistentComponent<PortalCooldownDurationComponent>();
        component->current_portal_cooldown_ticks = portal_cooldown;
        if (component->portal_block_pos != BlockPos::MAX) {
            const auto &portal = getDimensionBlockSource().getBlock(component->portal_block_pos);
            axis = portal.getState<PortalAxis>(VanillaStateIds::PortalAxis);
        }
    }
    auto &component = entity_context_.getOrAddComponent<DimensionTransitionComponent>();
    component.portal_entrance_position = getPosition();
    component.portal_entrance_axis = axis;
    component.existing_target_position.reset();
}

// void Actor::_setHeightOffset(float offset)
// {
//     ActorOffset::setHeightOffset(getEntity(), offset);
//     auto pos = getPosition();
//     setAABB(ActorSetPosSystem::refreshAABB(offset, pos, getAABBDim()));
//     _moveHitboxTo(pos);
// }

// void Actor::_moveHitboxTo(const Vec3 &position)
// {
//     if (auto *component = tryGetComponent<HitboxComponent>(); component != nullptr) {
//         ActorSetPosSystem::moveHitboxTo(position, component->hitboxes);
//     }
// }

bool Actor::isJumping() const
{
    return MobJump::isJumping(entity_context_);
}

std::vector<std::string> Actor::getTags() const
{
    auto *component = tryGetComponent<TagsComponent<IDType<LevelTagSetIDType>>>();
    if (!component) {
        return {};
    }
    return getLevel().getTagRegistry().getTagsInSet(component->tag_set_id);
}

bool Actor::addTag(const std::string &tag)
{
    if (!level_) {
        return false;
    }
    auto &component = entity_context_.getOrAddComponent<TagsComponent<IDType<LevelTagSetIDType>>>();
    auto &tag_registry = getLevel().getTagRegistry();
    if (TagSystem::hasTag(component, tag, tag_registry)) {
        return false;
    }
    getLevel().incrementTagCache(tag, tag_registry);
    component.tag_set_id = tag_registry.addTagToSet(tag, component.tag_set_id);
    return true;
}

bool Actor::removeTag(const std::string &tag)
{
    if (!level_) {
        return false;
    }
    auto &component = entity_context_.getOrAddComponent<TagsComponent<IDType<LevelTagSetIDType>>>();
    auto &tag_registry = getLevel().getTagRegistry();
    if (!TagSystem::hasTag(component, tag, tag_registry)) {
        return false;
    }
    getLevel().decrementTagCache(tag, tag_registry);
    TagSystem::removeTag(component, tag, tag_registry);
    return true;
}

int Actor::getHealth() const
{
    if (const auto *component = entity_context_.tryGetComponent<AttributesComponent>(); component) {
        const auto &instance = component->attributes.getInstance("minecraft:health");
        const auto current_value = instance->getCurrentValue();
        return static_cast<int>(std::ceilf(current_value));
    }
    return 0;
}

int Actor::getMaxHealth() const
{
    if (const auto *component = entity_context_.tryGetComponent<AttributesComponent>(); component) {
        const auto &instance = component->attributes.getInstance("minecraft:health");
        const auto current_value = instance->getMaxValue();
        return static_cast<int>(std::ceilf(current_value));
    }
    return 0;
}

void Actor::setNameTagVisible(bool visible)
{
    setStatusFlag(ActorFlags::CAN_SHOW_NAME, visible);
}

const std::string &Actor::getNameTag() const
{
    return entity_data.getString(static_cast<SynchedActorData::ID>(ActorDataIDs::NAME));
}

void Actor::setNameTag(const std::string &name)
{
    entity_data.set(static_cast<SynchedActorData::ID>(ActorDataIDs::NAME), name);
    name_tag_hash = HashedString(name).getHash();
}

const std::string &Actor::getScoreTag() const
{
    return entity_data.getString(static_cast<SynchedActorData::ID>(ActorDataIDs::SCORE));
}

void Actor::setScoreTag(const std::string &score)
{
    entity_data.set(static_cast<SynchedActorData::ID>(ActorDataIDs::SCORE), score);
}

const AttributeInstance *Actor::getAttribute(const HashedString &name) const
{
    auto component = getPersistentComponent<AttributesComponent>();
    return component->attributes.getInstance(name);
}

MutableAttributeWithContext Actor::getMutableAttribute(const HashedString &name)
{
    auto component = getPersistentComponent<AttributesComponent>();
    return component->attributes.getMutableInstanceWithContext(name);
}

float Actor::getFallDistance() const
{
    auto component = getPersistentComponent<FallDistanceComponent>();
    return component->value;
}

void Actor::setFallDistance(float value)
{
    getPersistentComponent<FallDistanceComponent>()->value = value;
}

bool Actor::isDead() const
{
    return hasComponent<IsDeadFlagComponent>();
}

EntityContext &Actor::getEntity()
{
    return entity_context_;
}

const EntityContext &Actor::getEntity() const
{
    return entity_context_;
}

WeakRef<EntityContext> Actor::getWeakEntity() const
{
    return entity_context_.getWeakRef();
}

const ItemStack &Actor::getOffhandSlot() const
{
    auto component = getPersistentComponent<ActorEquipmentComponent>();
    return component->hand->getItem(static_cast<int>(HandSlot::Offhand));
}

const ItemStack &Actor::getArmor(ArmorSlot slot) const
{
    auto component = getPersistentComponent<ActorEquipmentComponent>();
    return component->armor->getItem(static_cast<int>(slot));
}

bool Actor::isCreative() const
{
    if (const auto *component = tryGetComponent<ActorGameTypeComponent>()) {
        return component->game_type == GameType::Creative || (component->game_type == GameType::WorldDefault &&
                                                              getLevel().getDefaultGameType() == GameType::Creative);
    }
    return false;
}

bool Actor::isSpectator() const
{
    if (const auto *component = tryGetComponent<ActorGameTypeComponent>()) {
        return component->game_type == GameType::Spectator || (component->game_type == GameType::WorldDefault &&
                                                               getLevel().getDefaultGameType() == GameType::Spectator);
    }
    return false;
}

void Actor::queueBBUpdateFromValue(const Vec2 &value)
{
    getEntity().getOrAddComponent<ShouldUpdateBoundingBoxRequestComponent>().update =
        ShouldUpdateBoundingBoxRequestComponent::UpdateFromValue(value);
}

void Actor::queueBBUpdateFromDefinition()
{
    getEntity().getOrAddComponent<ShouldUpdateBoundingBoxRequestComponent>().update =
        ShouldUpdateBoundingBoxRequestComponent::UpdateFromDefinition();
}

bool Actor::getChainedDamageEffects() const
{
    return chained_damage_effects_;
}

void Actor::_serializeComponents(CompoundTag &) const
{
    // TODO(killcerr): Use BEDROCK_CALL to call the original function once the symbol is added
    // The function iterates through the DefinitionInstanceGroup (at Actor+0x90)
    // and calls IDefinitionInstance::_save(EntityContext const&, CompoundTag&) on
    // each registered definition (ExplodeDefinition, AgeableDefinition, etc.)
}

ActorUniqueID Actor::getOwnerId() const
{
    return ActorUniqueID(entity_data.getInt64(static_cast<SynchedActorData::ID>(ActorDataIDs::OWNER)));
}

void Actor::saveEntityFlags(CompoundTag &entity_tag) const
{
    entity_tag.putBoolean("LootDropped", loot_dropped_);
    entity_tag.putByte("Color", entity_data.getInt8(static_cast<SynchedActorData::ID>(ActorDataIDs::COLOR_INDEX)));
    entity_tag.putByte("Color2", entity_data.getInt8(static_cast<SynchedActorData::ID>(ActorDataIDs::COLOR_2_INDEX)));
    entity_tag.putInt("Strength", entity_data.getInt(static_cast<SynchedActorData::ID>(ActorDataIDs::STRENGTH)));
    entity_tag.putInt("StrengthMax", entity_data.getInt(static_cast<SynchedActorData::ID>(ActorDataIDs::STRENGTH_MAX)));
    entity_tag.putBoolean("Sheared", getStatusFlag(ActorFlags::SHEARED));
    entity_tag.putBoolean("IsIllagerCaptain", getStatusFlag(ActorFlags::IS_ILLAGER_CAPTAIN));
    entity_tag.putInt64("OwnerNew", getOwnerId().raw_id);
    entity_tag.putBoolean("Sitting", getStatusFlag(ActorFlags::SITTING));
    entity_tag.putBoolean("IsBaby", getStatusFlag(ActorFlags::BABY));
    entity_tag.putBoolean("IsTamed", getStatusFlag(ActorFlags::TAMED));
    entity_tag.putBoolean("IsTrusting", getStatusFlag(ActorFlags::TRUSTING));
    entity_tag.putBoolean("IsOrphaned", getStatusFlag(ActorFlags::ORPHANED));
    entity_tag.putBoolean("IsAngry", getStatusFlag(ActorFlags::ANGRY));
    entity_tag.putBoolean("IsOutOfControl", getStatusFlag(ActorFlags::OUT_OF_CONTROL));
    entity_tag.putInt("Variant", entity_data.getInt(static_cast<SynchedActorData::ID>(ActorDataIDs::VARIANT)));
    entity_tag.putInt("MarkVariant", entity_data.getInt(static_cast<SynchedActorData::ID>(ActorDataIDs::MARK_VARIANT)));
    entity_tag.putBoolean("Saddled", getStatusFlag(ActorFlags::SADDLED));
    entity_tag.putBoolean("Chested", getStatusFlag(ActorFlags::CHESTED));
    entity_tag.putBoolean("ShowBottom", getStatusFlag(ActorFlags::SHOW_BOTTOM));
    entity_tag.putBoolean("IsGliding", getStatusFlag(ActorFlags::GLIDING));
    entity_tag.putBoolean("IsSwimming", getStatusFlag(ActorFlags::SWIMMING));
    entity_tag.putBoolean("IsEating", getStatusFlag(ActorFlags::EATING));
    entity_tag.putBoolean("IsScared", getStatusFlag(ActorFlags::SCARED));
    entity_tag.putBoolean("IsStunned", getStatusFlag(ActorFlags::STUNNED));
    entity_tag.putBoolean("IsRoaring", getStatusFlag(ActorFlags::ROARING));
    entity_tag.putInt("SkinID", entity_data.getInt(static_cast<SynchedActorData::ID>(ActorDataIDs::SKIN_ID)));
}

std::unique_ptr<ListTag> Actor::saveLinks() const
{
    auto links = std::make_unique<ListTag>();
    int index = 0;
    for (const auto &passenger : ActorRiding::getPassengers(getEntity())) {
        auto link = CompoundTag();
        link.putInt64("entityID", passenger.actor_id.raw_id);
        link.putInt("linkID", index);
        links->add(std::make_unique<CompoundTag>(std::move(link)));
        ++index;
    }
    return links;
}

void Actor::saveWithoutId(CompoundTag &entity_tag) const
{
    // UniqueID
    entity_tag.putInt64("UniqueID", getOrCreateUniqueID().raw_id);

    // Pos - [x, y, z] (float list)
    {
        auto pos_list = ListTag();
        auto pos = getPosition();
        pos_list.add(std::make_unique<FloatTag>(pos.x));
        pos_list.add(std::make_unique<FloatTag>(pos.y));
        pos_list.add(std::make_unique<FloatTag>(pos.z));
        entity_tag.put("Pos", std::move(pos_list));
    }

    // Rotation - [yaw, pitch]
    {
        auto rot_list = ListTag();
        auto rot = getRotation();
        rot_list.add(std::make_unique<FloatTag>(rot.x));
        rot_list.add(std::make_unique<FloatTag>(rot.y));
        entity_tag.put("Rotation", std::move(rot_list));
    }

    // Motion - [dx, dy, dz] (float list)
    {
        auto motion_list = ListTag();
        auto motion = getPosDelta();
        motion_list.add(std::make_unique<FloatTag>(motion.x));
        motion_list.add(std::make_unique<FloatTag>(motion.y));
        motion_list.add(std::make_unique<FloatTag>(motion.z));
        entity_tag.put("Motion", std::move(motion_list));
    }

    // CustomName
    auto name = getNameTag();
    if (!name.empty()) {
        entity_tag.putString("CustomName", name);
        entity_tag.putBoolean("CustomNameVisible", getStatusFlag(ActorFlags::CAN_SHOW_NAME));
    }

    // FallDistance
    entity_tag.putFloat("FallDistance", getFallDistance());

    // OnGround
    entity_tag.putByte("OnGround", isOnGround() ? 1 : 0);

    // Invulnerable
    entity_tag.putByte("Invulnerable", invulnerable ? 1 : 0);

    // PortalCooldown
    if (auto *component = tryGetComponent<PortalCooldownDurationComponent>(); component) {
        entity_tag.putInt("PortalCooldown", component->current_portal_cooldown_ticks);
    }

    // IsGlobal
    entity_tag.putBoolean("IsGlobal", hasComponent<GlobalActorComponent>());

    // IsAutonomous / LastDimensionId
    auto is_autonomous = hasComponent<AutonomousActorComponent>();
    entity_tag.putBoolean("IsAutonomous", is_autonomous);
    if (is_autonomous) {
        entity_tag.putInt("LastDimensionId", getDimensionId().value);
    }

    // LinksTag
    {
        auto links = saveLinks();
        if (links && links->size() > 0) {
            entity_tag.put("LinksTag", std::move(*links));
        }
    }

    // Entity flags
    saveEntityFlags(entity_tag);

    // Subclass-specific data
    const_cast<Actor *>(this)->addAdditionalSaveData(entity_tag);
}

bool Actor::save(CompoundTag &entity_tag) const
{
    if (!definition_list_) {
        return false;
    }

    const auto &stack = definition_list_->getDefinitionStack();
    auto first_name = getActorIdentifier().getCanonicalName();

    // identifier — from first stack entry's descriptor
    if (!stack.empty() && stack[0].desc) {
        // Read mRuntimeIdentifier.mRuntimeId from descriptor at offset 48
        const auto *desc_ptr = reinterpret_cast<const char *>(stack[0].desc);
        const auto &rt_id = *reinterpret_cast<const std::string *>(desc_ptr + 48);
        if (!rt_id.empty()) {
            first_name = rt_id;
        }
    }
    entity_tag.putString("identifier", first_name);

    // definitions
    {
        auto def_list = ListTag();
        for (const auto &entry : stack) {
            const auto *desc_ptr = reinterpret_cast<const char *>(entry.desc);
            const auto &rt_id = *reinterpret_cast<const std::string *>(desc_ptr + 48);
            def_list.add(std::make_unique<StringTag>(std::string(entry.add ? "+" : "-") + rt_id));
        }
        entity_tag.put("definitions", std::move(def_list));
    }

    saveWithoutId(entity_tag);
    _serializeComponents(entity_tag);

    // TODO(killcerr): EconomyTradeableComponent::addAdditionalSaveData

    return true;
}
