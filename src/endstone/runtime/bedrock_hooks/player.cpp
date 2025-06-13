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

#include <iostream>

#include <entt/entt.hpp>

#include "bedrock/entity/components/block_source_component.h"
#include "bedrock/entity/components/movement_interpolator_component.h"
#include "bedrock/entity/components/replay_state_component.h"
#include "bedrock/world/actor/provider/player_sleep.h"
#include "bedrock/world/level/block/actor/bed_block_actor.h"
#include "bedrock/world/level/block/actor/block_actor.h"
#include "bedrock/world/level/block/actor/block_actor_type.h"
#include "bedrock/world/level/block/bed_block.h"
#include "bedrock/world/level/difficulty.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bed_enter_event.h"
#include "endstone/event/player/player_bed_leave_event.h"
#include "endstone/event/player/player_drop_item_event.h"
#include "endstone/event/player/player_item_consume_event.h"
#include "endstone/event/player/player_teleport_event.h"
#include "endstone/runtime/hook.h"

void Player::teleportTo(const Vec3 &pos, bool should_stop_riding, int cause, int entity_type, bool keep_velocity)
{
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const endstone::Location to{&player.getDimension(), pos.x, pos.y, pos.z, getRotation().x, getRotation().y};
    endstone::PlayerTeleportEvent e{player, player.getLocation(), to};
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
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    const auto item = endstone::core::EndstoneItemStack::fromMinecraft(item_in_use_.getItemInUse());
    const auto hand = inventory_->getSelectedSlot().container_id == CONTAINER_ID_INVENTORY
                        ? endstone::EquipmentSlot::Hand
                        : endstone::EquipmentSlot::OffHand;
    endstone::PlayerItemConsumeEvent e{getEndstoneActor<endstone::core::EndstonePlayer>(), *item, hand};
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
    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const auto drop = endstone::core::EndstoneItemStack::fromMinecraft(item);
    endstone::PlayerDropItemEvent e(player, *drop);
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Player::drop, this, item, randomly);
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

        setBedRespawnPosition(bed_pos);
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

BedSleepingResult Player::startSleepInBed(BlockPos const &bed_block_pos)
{
    return startSleepInBed(bed_block_pos, false);
}

BedSleepingResult Player::startSleepInBed(BlockPos const &bed_block_pos, bool force)
{
    auto bed_result = getBedResult(bed_block_pos);
    if (bed_result == BedSleepingResult::OTHER_PROBLEM) {
        return bed_result;  // return immediately if the result is not bypass-able
    }

    if (force) {
        bed_result = BedSleepingResult::OK;
    }

    auto bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::OtherProblem;
    switch (bed_result) {
    case BedSleepingResult::OK:
        bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::Ok;
        break;
    case BedSleepingResult::NOT_POSSIBLE_HERE:
        bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::NotPossibleHere;
        break;
    case BedSleepingResult::NOT_POSSIBLE_NOW:
        bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::NotPossibleNow;
        break;
    case BedSleepingResult::TOO_FAR_AWAY:
        bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::TooFarAway;
        break;
    case BedSleepingResult::NOT_SAFE:
        bed_enter_result = endstone::PlayerBedEnterEvent::BedEnterResult::NotSafe;
        break;
    default:
        break;
    }

    const auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    const auto block = endstone::core::EndstoneBlock::at(getDimensionBlockSource(), bed_block_pos);

    endstone::PlayerBedEnterEvent e(player, *block, bed_enter_result);
    server.getPluginManager().callEvent(e);

    if (e.useBed() == endstone::EventResult::Allow) {
        bed_result = BedSleepingResult::OK;
    }
    else if (e.useBed() == endstone::EventResult::Deny) {
        bed_result = BedSleepingResult::OTHER_PROBLEM;
    }

    if (bed_result == BedSleepingResult::OK) {
        startSleeping(bed_block_pos);
    }
    return bed_result;
}

void Player::startSleeping(BlockPos const &bed_block_pos)
{
    if (isRiding()) {
        stopRiding(true, false, false, false);
    }
    if (isGliding()) {
        stopGliding();
    }
    if (hasPassenger()) {
        removeAllPassengers(false);
    }

    setBedRespawnPosition(bed_block_pos);
    entity_data.set<BlockPos>(static_cast<SynchedActorData::ID>(ActorDataIDs::BED_POSITION), bed_block_pos);

    if (auto *replay = tryGetComponent<ReplayStateComponent>()) {
        replay->clearHistory();
    }

    queueBBUpdateFromValue(Vec2(0.2F, 0.2F));
    _setHeightOffset(0.2F);

    const BlockSource *block_source = nullptr;
    if (const auto *component = tryGetComponent<BlockSourceComponent>(); component) {
        if (auto ref = component->tryGetBlockSource(); ref) {
            block_source = &ref.value();
        }
    }

    if (block_source == nullptr || !block_source->hasBlock(bed_block_pos)) {
        setPos({bed_block_pos.x + 0.5f, bed_block_pos.y + 0.0625f, bed_block_pos.z + 0.5f});
    }
    else {
        auto &block = block_source->getBlock(bed_block_pos);
        switch (block.getState<int>("direction")) {
        case Direction::SOUTH:
            setRotationWrapped({0.0f, 0.0f});
            break;
        case Direction::WEST:
            setRotationWrapped({0.0f, 90.0f});
            break;
        case Direction::NORTH:
            setRotationWrapped({0.0f, 180.0f});
            break;
        case Direction::EAST:
            setRotationWrapped({0.0f, -90.0f});
            break;
        default:
            break;
        }
        setPos({bed_block_pos.x + 0.5f, bed_block_pos.y + 0.90625F, bed_block_pos.z + 0.5f});
        setSprinting(false);
    }

    if (auto *interp = tryGetComponent<MovementInterpolatorComponent>()) {
        interp->reset();
    }

    setSleeping(true);
    PlayerSleep::resetSleepCounter(getEntity());
    setPosDelta(Vec3::ZERO);
    if (!getLevel().isClientSide()) {
        getLevel().updateSleepingPlayerList();
    }

    entity_data.setFlag<signed char>(static_cast<SynchedActorData::ID>(ActorDataIDs::PLAYER_FLAGS), 1);

    if (auto *block_actor = getDimensionBlockSource().getBlockEntity(bed_block_pos)) {
        if (block_actor->isType(BlockActorType::Bed)) {
            const_cast<BedBlockActor *>(static_cast<const BedBlockActor *>(block_actor))->startSleepingOn();
        }
    }
    setFallDistance(0.0F);
}

void Player::stopSleepInBed(bool forceful_wake_up, bool update_level_list)
{
    if (!isSleeping()) {
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();

    std::unique_ptr<endstone::Block> bed;
    if (hasBedPosition()) {
        const auto bed_position = getBedPosition();
        bed = player.getDimension().getBlockAt(bed_position.x, bed_position.y, bed_position.z);
    }
    else {
        bed = player.getDimension().getBlockAt(player.getLocation());
    }

    endstone::PlayerBedLeaveEvent e(player, *bed);
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::stopSleepInBed, this, forceful_wake_up, update_level_list);
}
