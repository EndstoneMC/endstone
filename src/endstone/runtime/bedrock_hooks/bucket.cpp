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

#include "bedrock/world/item/bucket_item.h"

#include <optional>
#include <string_view>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_descriptor.h"
#include "bedrock/world/level/block/cauldron_block.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bucket_empty_event.h"
#include "endstone/event/player/player_bucket_fill_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/runtime/hook.h"

namespace {

class BucketItemLayout : public ::Item {
public:
    [[nodiscard]] BucketFillType getFillType() const { return fill_type_; }

private:
    BucketFillType fill_type_;
};

BucketFillType getBucketFillType(const ::Item &item)
{
    return item.isBucket() ? static_cast<const BucketItemLayout &>(item).getFillType() : BucketFillType::Unknown;
}

bool isEmptyBucket(const ::Item &item)
{
    return getBucketFillType(item) == BucketFillType::Empty;
}

bool isFilledBucket(const ::Item &item)
{
    const auto fill_type = getBucketFillType(item);
    return fill_type != BucketFillType::Unknown && fill_type != BucketFillType::Empty &&
           fill_type != BucketFillType::Milk;
}

bool isCauldron(const ::Block &block)
{
    return block.getName().getString() == "minecraft:cauldron";
}

bool hasWaterContent(const ::Item &item)
{
    switch (getBucketFillType(item)) {
    case BucketFillType::Fish:
    case BucketFillType::Salmon:
    case BucketFillType::Tropicalfish:
    case BucketFillType::Pufferfish:
    case BucketFillType::Water:
    case BucketFillType::Axolotl:
    case BucketFillType::Tadpole:
    case BucketFillType::SulfurCube:
        return true;
    default:
        return false;
    }
}

bool isCauldronBucket(const ::Item &item)
{
    switch (getBucketFillType(item)) {
    case BucketFillType::Water:
    case BucketFillType::Lava:
    case BucketFillType::PowderSnow:
        return true;
    default:
        return false;
    }
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

std::optional<std::string_view> getFilledBucketName(const ::Block &block)
{
    static const HashedString liquid_depth{"liquid_depth"};
    const auto &name = block.getName().getString();
    if (name == "minecraft:water") {
        if (block.getState<int>(liquid_depth) != 0) {
            return std::nullopt;
        }
        return "minecraft:water_bucket";
    }
    if (name == "minecraft:lava") {
        if (block.getState<int>(liquid_depth) != 0) {
            return std::nullopt;
        }
        return "minecraft:lava_bucket";
    }
    if (name == "minecraft:powder_snow") {
        return "minecraft:powder_snow_bucket";
    }
    return std::nullopt;
}

std::optional<std::string_view> getCauldronBucketName(const ::Block &block)
{
    static const HashedString fill_level{"fill_level"};
    if (block.getState<int>(fill_level) != 6) {
        return std::nullopt;
    }

    static const HashedString cauldron_liquid{"cauldron_liquid"};
    switch (block.getState<int>(cauldron_liquid)) {
    case 0:
        return "minecraft:water_bucket";
    case 1:
        return "minecraft:lava_bucket";
    case 2:
        return "minecraft:powder_snow_bucket";
    default:
        return std::nullopt;
    }
}

const ::Block *getFluidBlock(const ::Item &item)
{
    if (hasWaterContent(item)) {
        return ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor("minecraft:water", std::nullopt)
            .tryGetBlockNoLogging();
    }
    if (getBucketFillType(item) == BucketFillType::Lava) {
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

bool isNativeResult(const std::optional<endstone::ItemStack> &item_stack, const ::ItemStack &expected)
{
    return item_stack && endstone::core::EndstoneItemStack::toMinecraft(*item_stack) == expected;
}

::ItemStack toMinecraftOrEmpty(const std::optional<endstone::ItemStack> &item_stack)
{
    return item_stack ? endstone::core::EndstoneItemStack::toMinecraft(*item_stack) : ::ItemStack::EMPTY_ITEM;
}

thread_local std::optional<BlockPos> cancelled_cauldron_click;

}  // namespace

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    cancelled_cauldron_click.reset();

    auto &player = event_data.player;
    const auto slot = player.getSelectedItemSlot();
    const auto &item_stack = player.getInventory().getItem(slot);
    const auto *item = item_stack.getItem();
    auto &block_source = player.getDimensionBlockSource();
    const auto &block = block_source.getBlock(event_data.pos);
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(event_data.face);

    const auto filling = item && block_face && isEmptyBucket(*item);
    const auto emptying = item && block_face && isCauldronBucket(*item) && canFillCauldron(block, *item);

    std::optional<::ItemStack> result_item;
    if (filling) {
        if (const auto bucket_name = getCauldronBucketName(block); bucket_name) {
            result_item = ::ItemStack(*bucket_name);
        }
    }
    else if (emptying) {
        result_item = ::ItemStack("minecraft:bucket");
    }

    if (!result_item) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto block_handle = endstone::core::EndstoneBlock::at(block_source, event_data.pos);
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
    auto cancelled = false;
    std::optional<endstone::ItemStack> replacement;
    if (filling) {
        endstone::PlayerBucketFillEvent event{
            player.getEndstoneActor<endstone::core::EndstonePlayer>(),
            block_handle,
            block_handle,
            *block_face,
            bucket_stack.getType(),
            endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
            endstone::EquipmentSlot::Hand,
        };
        server.getPluginManager().callEvent(event);
        cancelled = event.isCancelled();
        replacement = event.getItemStack();
    }
    else {
        endstone::PlayerBucketEmptyEvent event{
            player.getEndstoneActor<endstone::core::EndstonePlayer>(),
            block_handle,
            block_handle,
            *block_face,
            bucket_stack.getType(),
            endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
            endstone::EquipmentSlot::Hand,
        };
        server.getPluginManager().callEvent(event);
        cancelled = event.isCancelled();
        replacement = event.getItemStack();
    }

    if (cancelled) {
        event_data.successful = false;
        cancelled_cauldron_click = event_data.pos;
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
    if (event_data.successful.value_or(false) && !isNativeResult(replacement, *result_item)) {
        player.getInventory().setItem(slot, toMinecraftOrEmpty(replacement));
    }
}

InteractionResult BucketItem::_useOn(::ItemStack &item_stack, ::Actor &actor, BlockPos position, FacingID face,
                                     const Vec3 &click_pos) const
{
    auto &block_source = actor.getDimensionBlockSource();
    if (isCauldron(block_source.getBlock(position))) {
        if (cancelled_cauldron_click == position) {
            cancelled_cauldron_click.reset();
            return InteractionResult::Failure();
        }
        return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    }

    const auto *item = item_stack.getItem();
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(face);
    if (!item || !block_face || !actor.isPlayer()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    }

    auto *player = static_cast<::Player *>(&actor);
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
    auto cancelled = false;
    std::optional<::ItemStack> result_item;
    std::optional<endstone::ItemStack> replacement;
    if (isEmptyBucket(*item)) {
        const auto bucket_name = getFilledBucketName(block_source.getBlock(position));
        if (!bucket_name) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
        }

        result_item = ::ItemStack(*bucket_name);
        auto block_handle = endstone::core::EndstoneBlock::at(block_source, position);
        endstone::PlayerBucketFillEvent event{
            player->getEndstoneActor<endstone::core::EndstonePlayer>(),
            block_handle,
            block_handle,
            *block_face,
            bucket_stack.getType(),
            endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
            endstone::EquipmentSlot::Hand,
        };
        server.getPluginManager().callEvent(event);
        cancelled = event.isCancelled();
        replacement = event.getItemStack();
    }
    else if (isFilledBucket(*item)) {
        const auto empty_position = getEmptyPosition(block_source, position, face, *item);
        if (!empty_position) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
        }

        result_item = ::ItemStack("minecraft:bucket");
        endstone::PlayerBucketEmptyEvent event{
            player->getEndstoneActor<endstone::core::EndstonePlayer>(),
            endstone::core::EndstoneBlock::at(block_source, *empty_position),
            endstone::core::EndstoneBlock::at(block_source, position),
            *block_face,
            bucket_stack.getType(),
            endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
            endstone::EquipmentSlot::Hand,
        };
        server.getPluginManager().callEvent(event);
        cancelled = event.isCancelled();
        replacement = event.getItemStack();
    }
    else {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    }

    if (cancelled) {
        return InteractionResult::Failure();
    }

    const auto result =
        ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    if (result.isSuccessful() && !isNativeResult(replacement, *result_item)) {
        item_stack.setUserData(nullptr);
        item_stack = toMinecraftOrEmpty(replacement);
    }
    return result;
}
