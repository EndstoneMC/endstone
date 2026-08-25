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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/bucket_fill_type.h"
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

thread_local std::optional<BlockPos> cancelled_cauldron_click;

}  // namespace

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    cancelled_cauldron_click.reset();

    auto &player = event_data.player;
    const auto slot = player.getSelectedItemSlot();
    const auto &item_stack = player.getInventory().getItem(slot);
    const auto *item = item_stack.getItem();
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(event_data.face);
    if (!item || !item->isBucket() || !block_face) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
        return;
    }

    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    auto &block_source = player.getDimensionBlockSource();
    const auto &block = block_source.getBlock(event_data.pos);
    const auto fill_type = static_cast<const BucketItemLayout &>(*item).getFillType();
    const auto level = block.getState<int>(fill_level);
    const auto liquid = block.getState<int>(cauldron_liquid);
    const auto filling = fill_type == BucketFillType::Empty;

    std::optional<::ItemStack> result_item;
    if (filling && level == 6) {
        switch (liquid) {
        case 0:
            result_item = ::ItemStack("minecraft:water_bucket");
            break;
        case 1:
            result_item = ::ItemStack("minecraft:lava_bucket");
            break;
        case 2:
            result_item = ::ItemStack("minecraft:powder_snow_bucket");
            break;
        default:
            break;
        }
    }
    else if ((fill_type == BucketFillType::Water || fill_type == BucketFillType::Lava ||
              fill_type == BucketFillType::PowderSnow) &&
             (level == 0 || (level != 6 && fill_type == BucketFillType::Water && liquid == 0))) {
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
    if (!event_data.successful.value_or(false)) {
        return;
    }
    if (!replacement || endstone::core::EndstoneItemStack::toMinecraft(*replacement) != *result_item) {
        player.getInventory().setItem(
            slot, replacement ? endstone::core::EndstoneItemStack::toMinecraft(*replacement) : ::ItemStack::EMPTY_ITEM);
    }
}

InteractionResult BucketItem::_useOn(::ItemStack &item_stack, ::Actor &actor, BlockPos position, FacingID face,
                                     const Vec3 &click_pos) const
{
    auto &block_source = actor.getDimensionBlockSource();
    const auto &clicked_block = block_source.getBlock(position);
    if (clicked_block.getName().getString() == "minecraft:cauldron") {
        if (cancelled_cauldron_click == position) {
            cancelled_cauldron_click.reset();
            return InteractionResult::Failure();
        }
        return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    }

    const auto *item = item_stack.getItem();
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(face);
    if (!item || !item->isBucket() || !block_face || !actor.isPlayer()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
    }

    auto &player = static_cast<::Player &>(actor);
    const auto fill_type = static_cast<const BucketItemLayout &>(*item).getFillType();
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
    auto cancelled = false;
    std::optional<::ItemStack> result_item;
    std::optional<endstone::ItemStack> replacement;
    if (fill_type == BucketFillType::Empty) {
        static const HashedString liquid_depth{"liquid_depth"};
        const auto &block_name = clicked_block.getName().getString();
        if (block_name == "minecraft:powder_snow") {
            result_item = ::ItemStack("minecraft:powder_snow_bucket");
        }
        else if ((block_name == "minecraft:water" || block_name == "minecraft:lava") &&
                 clicked_block.getState<int>(liquid_depth) == 0) {
            result_item = ::ItemStack(block_name == "minecraft:water" ? "minecraft:water_bucket"
                                                                      : "minecraft:lava_bucket");
        }
        if (!result_item) {
            return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face, click_pos);
        }

        auto block_handle = endstone::core::EndstoneBlock::at(block_source, position);
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
    else if (fill_type != BucketFillType::Unknown && fill_type != BucketFillType::Milk) {
        const auto water_content =
            fill_type == BucketFillType::Water || fill_type == BucketFillType::Fish ||
            fill_type == BucketFillType::Salmon || fill_type == BucketFillType::Tropicalfish ||
            fill_type == BucketFillType::Pufferfish || fill_type == BucketFillType::Axolotl ||
            fill_type == BucketFillType::Tadpole || fill_type == BucketFillType::SulfurCube;
        const ::Block *fluid_block = nullptr;
        if (water_content) {
            fluid_block =
                ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor("minecraft:water", std::nullopt)
                    .tryGetBlockNoLogging();
        }
        else if (fill_type == BucketFillType::Lava) {
            fluid_block =
                ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor("minecraft:lava", std::nullopt)
                    .tryGetBlockNoLogging();
        }

        auto empty_position = position;
        if (!fluid_block || !water_content ||
            !clicked_block.getBlockType().canFillAtPos(block_source, position, *fluid_block)) {
            empty_position = position.offset(endstone::core::EndstoneBlockFace::getOffsetX(*block_face),
                                             endstone::core::EndstoneBlockFace::getOffsetY(*block_face),
                                             endstone::core::EndstoneBlockFace::getOffsetZ(*block_face));
            const auto &target_block = block_source.getBlock(empty_position);
            if (!block_source.isEmptyBlock(empty_position) &&
                !target_block.getBlockType().canBeBuiltOver(target_block, block_source, empty_position) &&
                !(fluid_block &&
                  target_block.getBlockType().canFillAtPos(block_source, empty_position, *fluid_block))) {
                return ENDSTONE_HOOK_CALL_ORIGINAL(&BucketItem::_useOn, this, item_stack, actor, position, face,
                                                   click_pos);
            }
        }

        result_item = ::ItemStack("minecraft:bucket");
        endstone::PlayerBucketEmptyEvent event{
            player.getEndstoneActor<endstone::core::EndstonePlayer>(),
            endstone::core::EndstoneBlock::at(block_source, empty_position),
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
    if (result.isSuccessful() &&
        (!replacement || endstone::core::EndstoneItemStack::toMinecraft(*replacement) != *result_item)) {
        item_stack.setUserData(nullptr);
        item_stack = replacement ? endstone::core::EndstoneItemStack::toMinecraft(*replacement)
                                 : ::ItemStack::EMPTY_ITEM;
    }
    return result;
}
