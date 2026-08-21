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

#include "endstone/runtime/bedrock_hooks/bucket_fill_entity.h"

#include <optional>
#include <utility>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/shared_types/legacy/facing.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/events/player_events.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block_pos.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bucket_fill_event.h"
#include "endstone/runtime/bedrock_hooks/actor_interaction.h"
#include "endstone/runtime/bedrock_hooks/bucket.h"

namespace {

struct PendingBucketFillEntity {
    const ::Player *player;
    const ::Actor *target;
    endstone::EquipmentSlot hand;
    std::optional<endstone::ItemStack> item_stack;
    bool write_item_stack;
};

thread_local std::optional<PendingBucketFillEntity> pending_bucket_fill_entity;

bool hasComponent(const ::Actor &actor, const char *name)
{
    return const_cast<::Actor &>(actor).hasComponent(HashedString(name));
}

bool isEmptyBucket(const ::ItemStack &item)
{
    return !item.isNull() && item.getItem() &&
           endstone::runtime::getBucketFillType(*item.getItem()) == BucketFillType::Empty;
}

bool isMilkBucket(const ::ItemStack &item)
{
    return !item.isNull() && item.getItem() &&
           endstone::runtime::getBucketFillType(*item.getItem()) == BucketFillType::Milk;
}

bool isMilkable(const ::Actor &target)
{
    const auto &name = target.getActorIdentifier().getCanonicalName();
    return (name == "minecraft:cow" || name == "minecraft:goat" || name == "minecraft:mooshroom") &&
           !hasComponent(target, "minecraft:is_baby");
}

BlockPos getBlockPosition(const ::Actor &target)
{
    const auto &position = target.getPosition();
    return {position.x, target.getAABB().min.y, position.z};
}

}  // namespace

namespace endstone::runtime {

void resetBucketFillEntityEvent()
{
    pending_bucket_fill_entity.reset();
}

bool fireBucketFillEntityEvent(::Player &player, const ::Actor &target, const ::ItemStack &item)
{
    if (!isEmptyBucket(item) || !isMilkable(target)) {
        return true;
    }

    auto &block_source = target.getDimension().getBlockSourceFromMainChunkSource();
    auto block_clicked = endstone::core::EndstoneBlock::at(block_source, getBlockPosition(target));
    const auto hand = getInteractionHand(player, item);
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item);
    auto item_stack = endstone::core::EndstoneItemStack::fromMinecraft(::ItemStack("minecraft:milk_bucket"));
    endstone::PlayerBucketFillEvent event{
        player.getEndstoneActor<endstone::core::EndstonePlayer>(),
        block_clicked.get(),
        *block_clicked,
        endstone::BlockFace::Self,
        bucket_stack.getType(),
        hand,
        std::move(item_stack),
    };
    event.setCancelled(
        !endstone::runtime::canBuild(block_source, player, getBlockPosition(target), Facing::NOT_DEFINED, item));
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return false;
    }

    const auto &event_item_stack = event.getItemStack();
    const ::ItemStack default_result{"minecraft:milk_bucket"};
    const auto keep_native_result =
        event_item_stack && endstone::core::EndstoneItemStack::toMinecraft(*event_item_stack) == default_result;
    pending_bucket_fill_entity = PendingBucketFillEntity{&player, &target, hand, event_item_stack, !keep_native_result};
    return true;
}

void handleBucketFillEntityResult(const PlayerInteractWithEntityAfterEvent &event)
{
    if (!pending_bucket_fill_entity) {
        return;
    }

    auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>();
    const auto *target = WeakEntityRef(event.target_entity).tryUnwrap<::Actor>();
    if (player != pending_bucket_fill_entity->player || target != pending_bucket_fill_entity->target) {
        pending_bucket_fill_entity.reset();
        return;
    }

    const auto succeeded =
        isMilkBucket(event.after_item) || (player->isCreative() && event.after_item == event.before_item);
    if (succeeded && pending_bucket_fill_entity->write_item_stack) {
        const auto item_stack =
            pending_bucket_fill_entity->item_stack
                ? endstone::core::EndstoneItemStack::toMinecraft(*pending_bucket_fill_entity->item_stack)
                : ::ItemStack::EMPTY_ITEM;
        if (pending_bucket_fill_entity->hand == endstone::EquipmentSlot::OffHand) {
            player->setOffhandSlot(item_stack);
        }
        else {
            player->setCarriedItem(item_stack);
        }
    }
    pending_bucket_fill_entity.reset();
}

}  // namespace endstone::runtime
