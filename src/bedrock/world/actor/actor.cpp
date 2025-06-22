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
#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/entity/components/actor_type_flag_component.h"
#include "bedrock/entity/components/actor_unique_id_component.h"
#include "bedrock/entity/components/fall_distance_component.h"
#include "bedrock/entity/components/is_dead_flag_component.h"
#include "bedrock/entity/components/passenger_component.h"
#include "bedrock/entity/components/player_component.h"
#include "bedrock/entity/components/runtime_id_component.h"
#include "bedrock/entity/components/tags_component.h"
#include "bedrock/entity/systems/tag_system.h"
#include "bedrock/entity/utilities/rotation_utility.h"
#include "bedrock/entity/utilities/synched_actor_data_access.h"
#include "bedrock/world/actor/actor_collision.h"
#include "bedrock/world/actor/actor_environment.h"
#include "bedrock/world/actor/armor_slot.h"
#include "bedrock/world/actor/mob_jump.h"
#include "bedrock/world/actor/player/player.h"
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

Dimension &Actor::getDimension() const
{
    return *dimension_.unwrap();
}

Level &Actor::getLevel()
{
    return *static_cast<Level *>(level_);
}

const Level &Actor::getLevel() const
{
    return *static_cast<Level *>(level_);
}

Vec3 const &Actor::getPosition() const
{
    return built_in_components_.state_vector_component->pos;
}

Vec3 const &Actor::getPosPrev() const
{
    return built_in_components_.state_vector_component->pos_prev;
}

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

bool Actor::hasCategory(ActorCategory categories) const
{
    return (categories & categories_) == categories;
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
        const auto current_value = instance.getCurrentValue();
        return static_cast<int>(std::ceilf(current_value));
    }
    return 0;
}

int Actor::getMaxHealth() const
{
    if (const auto *component = entity_context_.tryGetComponent<AttributesComponent>(); component) {
        const auto &instance = component->attributes.getInstance("minecraft:health");
        const auto current_value = instance.getMaxValue();
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

const AttributeInstance &Actor::getAttribute(const HashedString &name) const
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
