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

#include "bedrock/world/actor/player/player.h"

#include <optional>
#include <utility>

#include "bedrock/world/actor/actor_data_ids.h"
#include "bedrock/world/actor/item/item_actor.h"
#include "bedrock/world/level/block/bed_block.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/core/actor/item.h"
#include "endstone/core/block/block.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bed_enter_event.h"
#include "endstone/event/player/player_bed_leave_event.h"
#include "endstone/event/player/player_drop_item_event.h"
#include "endstone/event/player/player_exp_change_event.h"
#include "endstone/event/player/player_item_consume_event.h"
#include "endstone/event/player/player_level_change_event.h"
#include "endstone/event/player/player_pickup_arrow_event.h"
#include "endstone/event/player/player_pickup_item_event.h"
#include "endstone/event/player/player_set_spawn_event.h"
#include "endstone/event/player/player_teleport_event.h"
#include "endstone/runtime/hook.h"

namespace {

struct SpawnPosition {
    BlockPos position;
    DimensionType dimension;
    bool clear = false;
};

SpawnPosition get_spawn_position(const std::optional<endstone::Location> &location, const SpawnPosition fallback)
{
    if (!location) {
        return {BlockPos::MIN, VanillaDimensions::Undefined, true};
    }
    if (!location->isDimensionLoaded()) {
        return fallback;
    }

    const auto dimension = location->getDimension();
    const auto &endstone_dimension = static_cast<const endstone::core::EndstoneDimension &>(dimension.value());
    return {BlockPos(location->getX(), location->getY(), location->getZ()),
            endstone_dimension.getHandle().getDimensionId(), false};
}

std::optional<SpawnPosition> fire_set_spawn_event(Player &player, const BlockPos &position,
                                                  const DimensionType dimension,
                                                  endstone::PlayerSetSpawnEvent::Cause cause)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto endstone_player = player.getEndstoneActor<endstone::core::EndstonePlayer>();
    std::optional<endstone::Location> location;
    if (position != BlockPos::MIN || dimension != VanillaDimensions::Undefined) {
        location =
            endstone::Location{server.getEndstoneLevel()->getDimension(dimension), position.x, position.y, position.z};
    }

    endstone::PlayerSetSpawnEvent event{endstone_player, cause, std::move(location)};
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return std::nullopt;
    }

    return get_spawn_position(event.getLocation(), SpawnPosition{position, dimension});
}

}  // namespace

void Player::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    // Do not call PlayerTeleportEvent if EndstonePlayer::teleport was called internally
    // - (for example, when PlayerMoveEvent is cancelled).
    if (hasComponent<endstone::core::InternalTeleportFlagComponent>()) {
        addOrRemoveComponent<endstone::core::InternalTeleportFlagComponent>(false);
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::teleportTo, this, pos, should_stop_riding, cause, entity_type,
                                    keep_velocity);
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const endstone::Location to{player->getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
    endstone::PlayerTeleportEvent e{player, player->getLocation(), to};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    auto final_pos = Vec3(e.getTo().getX(), e.getTo().getY(), e.getTo().getZ());
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::teleportTo, this, final_pos, should_stop_riding, cause, entity_type,
                                keep_velocity);
}

void Player::completeUsingItem()
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto item = endstone::core::EndstoneItemStack::fromMinecraft(item_in_use_.getItemInUse());
    const auto hand = inventory_->getSelectedSlot().container_id == CONTAINER_ID_INVENTORY
                        ? endstone::EquipmentSlot::Hand
                        : endstone::EquipmentSlot::OffHand;
    endstone::PlayerItemConsumeEvent e{getEndstoneActor<endstone::core::EndstonePlayer>(), std::move(item), hand};
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        setStatusFlag(ActorFlags::USINGITEM, false);
        item_in_use_.clearItemInUse(getEntity());
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::completeUsingItem, this);
}

bool Player::drop(const ItemStack &item, bool randomly)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
    if (isAlive() && isPlayerInitialized()) {
        const auto drop = endstone::core::EndstoneItemStack::fromMinecraft(item);
        endstone::PlayerDropItemEvent e(player, drop);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::drop, this, item, randomly);
}

void Player::addExperience(int amount)
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerExpChangeEvent e{getEndstoneActor<endstone::core::EndstonePlayer>(), amount};
    server.getPluginManager().callEvent(e);
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::addExperience, this, e.getAmount());
}

void Player::addLevels(int levels)
{
    const auto old_level = getPlayerLevel();
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::addLevels, this, levels);
    const auto new_level = getPlayerLevel();
    if (old_level == new_level) {
        return;
    }
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerLevelChangeEvent e{getEndstoneActor<endstone::core::EndstonePlayer>(), old_level, new_level};
    server.getPluginManager().callEvent(e);
}

bool Player::take(Actor &actor, int unknown, int favored_slot)
{
    if (isClientSide() || !canInteractWithOtherEntitiesInGame()) {
        return false;
    }

    // TODO(refactor): replace with SAPI's ActorBeforeAcquireItemEvent?
    if (actor.hasCategory(ActorCategory::Item)) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
        auto item = actor.getEndstoneActor<endstone::core::EndstoneItem>();
        endstone::PlayerPickupItemEvent e(player, item);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    else if (actor.hasType(ActorType::AbstractArrow)) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
        auto arrow = actor.getEndstoneActor();
        endstone::PlayerPickupArrowEvent e(player, arrow);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::take, this, actor, unknown, favored_slot);
}

BedSleepingResult Player::getBedResult(const BlockPos &bed_pos)
{
    if (!isSleeping() && isAlive() && canSleep()) {
        const auto pos = getPosition();
        if (std::fabs(pos.x - static_cast<float>(bed_pos.x)) > 3.0F ||
            std::fabs(pos.y - static_cast<float>(bed_pos.y)) > 4.0F ||
            std::fabs(pos.z - static_cast<float>(bed_pos.z)) > 3.0F) {
            return BedSleepingResult::TOO_FAR_AWAY;
        }

        if (!getDimension().isNaturalDimension()) {
            return BedSleepingResult::NOT_POSSIBLE_HERE;
        }

        auto wakeup = BedBlock::findWakeupPosition(getDimensionBlockSource(), bed_pos, std::nullopt);
        if (!wakeup) {
            return BedSleepingResult::BED_OBSTRUCTED;
        }

        // setBedRespawnPosition(bed_pos);

        if (getDimension().isBrightOutside()) {
            return BedSleepingResult::NOT_POSSIBLE_NOW;
        }

        if (!isCreative() && getLevel().getDifficulty() != Difficulty::Peaceful) {
            const AABB bb(static_cast<float>(bed_pos.x) - 8.0F, static_cast<float>(bed_pos.y) - 5.0F,
                          static_cast<float>(bed_pos.z) - 8.0F, static_cast<float>(bed_pos.x) + 8.0F,
                          static_cast<float>(bed_pos.y) + 5.0F, static_cast<float>(bed_pos.z) + 8.0F);
            auto monsters = getDimensionBlockSource().fetchEntities(ActorType::Monster, bb, nullptr, nullptr);
            if (!monsters.empty()) {
                return BedSleepingResult::NOT_SAFE;
            }
        }

        return BedSleepingResult::OK;
    }

    return BedSleepingResult::OTHER_PROBLEM;
}

BedSleepingResult Player::startSleepInBed(BlockPos const &bed_block_pos, bool a2, float a3)
{
    auto bed_result = getBedResult(bed_block_pos);
    if (bed_result == BedSleepingResult::OK) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
        const auto block = endstone::core::EndstoneBlock::at(getDimensionBlockSource(), bed_block_pos);

        endstone::PlayerBedEnterEvent e(player, *block);
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return BedSleepingResult::OTHER_PROBLEM;
        }
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::startSleepInBed, this, bed_block_pos, a2, a3);
}

void Player::stopSleepInBed(bool forceful_wake_up, bool update_level_list)
{
    if (isSleeping()) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto player = getEndstoneActor<endstone::core::EndstonePlayer>();
        const auto bed_position =
            entity_data.getPosition(static_cast<SynchedActorData::ID>(ActorDataIDs::BED_POSITION));
        const auto block = endstone::core::EndstoneBlock::at(getDimensionBlockSource(), bed_position);

        endstone::PlayerBedLeaveEvent e(player, *block);
        server.getPluginManager().callEvent(e);
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::stopSleepInBed, this, forceful_wake_up, update_level_list);
}

bool Player::setSpawnBlockRespawnPosition(const BlockPos &spawn_block_position, DimensionType dimension)
{
    if (!isPlayerInitialized()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setSpawnBlockRespawnPosition, this, spawn_block_position,
                                           dimension);
    }

    auto cause = endstone::PlayerSetSpawnEvent::Cause::Unknown;
    if (hasComponent<endstone::core::InternalSpawnChangeFlagComponent>()) {
        addOrRemoveComponent<endstone::core::InternalSpawnChangeFlagComponent>(false);
        cause = endstone::PlayerSetSpawnEvent::Cause::Plugin;
    }
    else {
        const auto block_name = getDimensionBlockSource().getBlock(spawn_block_position).getName().getString();
        if (block_name == "minecraft:bed") {
            cause = endstone::PlayerSetSpawnEvent::Cause::Bed;
        }
        else if (block_name == "minecraft:respawn_anchor") {
            cause = endstone::PlayerSetSpawnEvent::Cause::RespawnAnchor;
        }
    }

    const auto position = fire_set_spawn_event(*this, spawn_block_position, dimension, cause);
    if (!position) {
        return false;
    }
    if (position->clear) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setRespawnPosition, this, BlockPos::MIN, VanillaDimensions::Undefined);
        return false;
    }

    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setSpawnBlockRespawnPosition, this, position->position,
                                       position->dimension);
}

void Player::setRespawnPosition(const BlockPos &respawn_position, DimensionType dimension)
{
    if (!isPlayerInitialized()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setRespawnPosition, this, respawn_position, dimension);
        return;
    }

    auto cause = endstone::PlayerSetSpawnEvent::Cause::Command;
    if (hasComponent<endstone::core::InternalSpawnChangeFlagComponent>()) {
        addOrRemoveComponent<endstone::core::InternalSpawnChangeFlagComponent>(false);
        cause = endstone::PlayerSetSpawnEvent::Cause::Plugin;
    }

    const auto position = fire_set_spawn_event(*this, respawn_position, dimension, cause);
    if (!position) {
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setRespawnPosition, this, position->position, position->dimension);
}
