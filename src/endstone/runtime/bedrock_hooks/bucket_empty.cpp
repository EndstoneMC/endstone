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

#include "endstone/runtime/bedrock_hooks/bucket_empty.h"

#include <optional>
#include <utility>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_descriptor.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bucket_empty_event.h"
#include "endstone/runtime/bedrock_hooks/actor_interaction.h"

namespace {

bool isFilledBucket(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    return item.isBucket() && name != "minecraft:bucket" && name != "minecraft:milk_bucket";
}

bool isCauldron(const ::Block &block)
{
    return block.getName().getString() == "minecraft:cauldron";
}

bool hasWaterContent(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    return name == "minecraft:water_bucket" || name == "minecraft:cod_bucket" ||
           name == "minecraft:salmon_bucket" || name == "minecraft:tropical_fish_bucket" ||
           name == "minecraft:pufferfish_bucket" || name == "minecraft:axolotl_bucket" ||
           name == "minecraft:tadpole_bucket" || name == "minecraft:sulfur_cube_bucket";
}

bool isCauldronBucket(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    return name == "minecraft:water_bucket" || name == "minecraft:lava_bucket" ||
           name == "minecraft:powder_snow_bucket";
}

bool canFillCauldron(const ::Block &block, const ::Item &item)
{
    static const HashedString fill_level{"fill_level"};
    const auto level = block.getState<int>(fill_level);
    if (level == 6) {
        return false;
    }
    if (level == 0) {
        return true;
    }

    static const HashedString cauldron_liquid{"cauldron_liquid"};
    return hasWaterContent(item) && block.getState<int>(cauldron_liquid) == 0;
}

bool isNonWaterCauldronBucket(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    return name == "minecraft:lava_bucket" || name == "minecraft:powder_snow_bucket";
}

bool shouldHandleEmptyEvent(const ::Item &item, const ::Block &clicked_block)
{
    const auto &name = item.getFullItemName();
    return isFilledBucket(item) &&
           (isCauldron(clicked_block) ? isCauldronBucket(item) : name != "minecraft:powder_snow_bucket");
}

const ::Block *getFluidBlock(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    if (hasWaterContent(item)) {
        return ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor("minecraft:water", std::nullopt)
            .tryGetBlockNoLogging();
    }
    if (name == "minecraft:lava_bucket") {
        return ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor("minecraft:lava", std::nullopt)
            .tryGetBlockNoLogging();
    }
    return nullptr;
}

std::optional<BlockPos> getEmptyPosition(::BlockSource &block_source, const BlockPos &clicked_position, FacingID face,
                                         const ::Item &item)
{
    const auto &clicked_block = block_source.getBlock(clicked_position);
    const auto *fluid_block = getFluidBlock(item);
    if (isCauldron(clicked_block)) {
        if (!isCauldronBucket(item)) {
            return std::nullopt;
        }
        if (!canFillCauldron(clicked_block, item)) {
            return std::nullopt;
        }
        return clicked_position;
    }

    if (fluid_block && hasWaterContent(item) &&
        clicked_block.getBlockType().canFillAtPos(block_source, clicked_position, *fluid_block)) {
        return clicked_position;
    }

    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(face);
    if (!block_face) {
        return std::nullopt;
    }

    const auto position = clicked_position.offset(endstone::core::EndstoneBlockFace::getOffsetX(*block_face),
                                                  endstone::core::EndstoneBlockFace::getOffsetY(*block_face),
                                                  endstone::core::EndstoneBlockFace::getOffsetZ(*block_face));
    const auto &block = block_source.getBlock(position);
    if (block_source.isEmptyBlock(position) || block.getBlockType().canBeBuiltOver(block, block_source, position) ||
        (fluid_block && block.getBlockType().canFillAtPos(block_source, position, *fluid_block))) {
        return position;
    }
    return std::nullopt;
}

struct BucketEmptyResult {
    ::Player *player;
    BlockPos position;
    std::optional<endstone::ItemStack> item_stack;
    bool write_item_stack;
};

thread_local std::optional<BucketEmptyResult> pending_bucket_empty;

endstone::runtime::BucketEmptyAction callBucketEmptyEvent(::BlockSource &block_source, ::Actor *placer,
                                                          const ::BlockPos &clicked_position, FacingID face,
                                                          const ::ItemStack &item_stack)
{
    if (!placer || !placer->isPlayer()) {
        return endstone::runtime::BucketEmptyAction::Continue;
    }

    auto *player = static_cast<::Player *>(placer);
    const auto *minecraft_item = item_stack.getItem();
    if (!minecraft_item) {
        return endstone::runtime::BucketEmptyAction::Continue;
    }

    const auto &clicked_block = block_source.getBlock(clicked_position);
    if (!shouldHandleEmptyEvent(*minecraft_item, clicked_block)) {
        return endstone::runtime::BucketEmptyAction::Continue;
    }

    const auto empty_position = getEmptyPosition(block_source, clicked_position, face, *minecraft_item);
    if (!empty_position) {
        return endstone::runtime::BucketEmptyAction::Continue;
    }

    if (isCauldron(clicked_block) && isNonWaterCauldronBucket(*minecraft_item) &&
        block_source.isUnderWater(clicked_position, clicked_block)) {
        return endstone::runtime::BucketEmptyAction::Consume;
    }

    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(face);
    if (!block_face) {
        return endstone::runtime::BucketEmptyAction::Continue;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto block = endstone::core::EndstoneBlock::at(block_source, *empty_position);
    auto block_clicked = endstone::core::EndstoneBlock::at(block_source, clicked_position);
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
    auto result_stack = endstone::core::EndstoneItemStack::fromMinecraft(::ItemStack("minecraft:bucket"));
    endstone::PlayerBucketEmptyEvent bucket_event{
        player->getEndstoneActor<endstone::core::EndstonePlayer>(),
        block.get(),
        *block_clicked,
        *block_face,
        bucket_stack.getType(),
        endstone::runtime::getInteractionHand(*player, item_stack),
        std::move(result_stack),
    };
    server.getPluginManager().callEvent(bucket_event);
    if (bucket_event.isCancelled()) {
        return endstone::runtime::BucketEmptyAction::Cancel;
    }

    const auto &event_item_stack = bucket_event.getItemStack();
    const ::ItemStack default_result{"minecraft:bucket"};
    const auto keep_native_result = event_item_stack &&
                                    endstone::core::EndstoneItemStack::toMinecraft(*event_item_stack) == default_result;
    pending_bucket_empty = BucketEmptyResult{
        player,
        clicked_position,
        event_item_stack,
        !keep_native_result,
    };
    return endstone::runtime::BucketEmptyAction::Continue;
}

}  // namespace

void endstone::runtime::cancelBucketEmptyResult()
{
    pending_bucket_empty.reset();
}

void endstone::runtime::handleCauldronBucketEmptyResult(::Player &player, const ::BlockPos &position, bool successful)
{
    if (!pending_bucket_empty) {
        return;
    }

    if (pending_bucket_empty->player != &player || pending_bucket_empty->position != position) {
        pending_bucket_empty.reset();
        return;
    }

    if (successful && pending_bucket_empty->write_item_stack) {
        const auto slot = player.getSelectedItemSlot();
        player.getInventory().setItem(
            slot, pending_bucket_empty->item_stack
                      ? endstone::core::EndstoneItemStack::toMinecraft(*pending_bucket_empty->item_stack)
                      : ::ItemStack::EMPTY_ITEM);
    }
    pending_bucket_empty.reset();
}

endstone::runtime::BucketEmptyAction endstone::runtime::handleBucketEmptyEvent(
    ::Actor &actor, const ::BlockPos &position, FacingID face, const ::ItemStack &item_stack)
{
    pending_bucket_empty.reset();
    return callBucketEmptyEvent(actor.getDimensionBlockSource(), &actor, position, face, item_stack);
}

void endstone::runtime::handleBucketEmptyResult(const ::InteractionResult &result, ::ItemStack &item_stack,
                                                 ::Actor &actor, const ::BlockPos &position)
{
    if (!pending_bucket_empty || !actor.isPlayer()) {
        return;
    }

    auto *player = static_cast<::Player *>(&actor);
    if (pending_bucket_empty->player != player || pending_bucket_empty->position != position) {
        pending_bucket_empty.reset();
        return;
    }

    if (result.isSuccessful() && pending_bucket_empty->write_item_stack) {
        item_stack.setUserData(nullptr);
        item_stack = pending_bucket_empty->item_stack
                         ? endstone::core::EndstoneItemStack::toMinecraft(*pending_bucket_empty->item_stack)
                         : ::ItemStack::EMPTY_ITEM;
    }
    pending_bucket_empty.reset();
}
