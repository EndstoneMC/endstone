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

#include "bedrock/world/level/block/cauldron_block.h"

#include <algorithm>
#include <optional>
#include <string_view>

#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/bucket_fill_type.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/cauldron_level_change_event.h"
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

}  // namespace

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    using ChangeReason = endstone::CauldronLevelChangeEvent::ChangeReason;

    auto &player = event_data.player;
    const auto slot = player.getSelectedItemSlot();
    const auto &item_stack = player.getInventory().getItem(slot);
    const auto *item = item_stack.getItem();
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(event_data.face);

    auto reason = ChangeReason::Unknown;
    auto filling = false;
    std::optional<::ItemStack> result_item;
    if (item && item->isBucket()) {
        static const HashedString fill_level{"fill_level"};
        static const HashedString cauldron_liquid{"cauldron_liquid"};
        const auto &block = player.getDimensionBlockSource().getBlock(event_data.pos);
        const auto fill_type = static_cast<const BucketItemLayout &>(*item).getFillType();
        const auto level = block.getState<int>(fill_level);
        const auto liquid = block.getState<int>(cauldron_liquid);
        filling = fill_type == BucketFillType::Empty;
        reason = filling ? ChangeReason::BucketFill : ChangeReason::BucketEmpty;
        if (block_face) {
            if (filling && level == MAX_FILL_LEVEL) {
                switch (static_cast<CauldronLiquidType>(liquid)) {
                case CauldronLiquidType::Water:
                    result_item = ::ItemStack("minecraft:water_bucket");
                    break;
                case CauldronLiquidType::Lava:
                    result_item = ::ItemStack("minecraft:lava_bucket");
                    break;
                case CauldronLiquidType::PowderSnow:
                    result_item = ::ItemStack("minecraft:powder_snow_bucket");
                    break;
                default:
                    break;
                }
            }
            else if ((fill_type == BucketFillType::Water || fill_type == BucketFillType::Lava ||
                      fill_type == BucketFillType::PowderSnow) &&
                     (level == MIN_FILL_LEVEL ||
                      (level != MAX_FILL_LEVEL && fill_type == BucketFillType::Water &&
                       liquid == static_cast<int>(CauldronLiquidType::Water)))) {
                result_item = ::ItemStack("minecraft:bucket");
            }
        }
    }
    else if (item) {
        const std::string_view name = item->getFullItemName();
        if (name == "minecraft:glass_bottle") {
            reason = ChangeReason::BottleFill;
        }
        else if (name == "minecraft:potion" || name == "minecraft:splash_potion" ||
                 name == "minecraft:lingering_potion") {
            reason = ChangeReason::BottleEmpty;
        }
        else if (name == "minecraft:banner") {
            reason = ChangeReason::BannerWash;
        }
        else if (name.ends_with("shulker_box")) {
            reason = ChangeReason::ShulkerWash;
        }
        else if (item->isDyeable() || name == "minecraft:wolf_armor") {
            reason = ChangeReason::ArmorWash;
        }
    }

    std::optional<endstone::ItemStack> replacement;
    if (result_item) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto block_handle = endstone::core::EndstoneBlock::at(player.getDimensionBlockSource(), event_data.pos);
        auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
        auto cancelled = false;
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
            event_data.successful = true;
            return;
        }
    }

    auto level_entity = player.getLevel().getLevelEntity().unwrap();
    if (level_entity) {
        auto &interaction = level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>();
        interaction.actor = player.getEndstoneActor<endstone::core::EndstonePlayer>();
        interaction.pos = event_data.pos;
        interaction.reason = reason;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
    if (level_entity) {
        level_entity->removeComponent<endstone::core::InternalCauldronInteractionComponent>();
    }

    if (!result_item || !event_data.successful.value_or(false)) {
        return;
    }
    if (!replacement || endstone::core::EndstoneItemStack::toMinecraft(*replacement) != *result_item) {
        player.getInventory().setItem(
            slot, replacement ? endstone::core::EndstoneItemStack::toMinecraft(*replacement) : ::ItemStack::EMPTY_ITEM);
    }
}

void CauldronBlock::setLiquidLevel(BlockSource &region, const BlockPos &pos, int liquid_level,
                                   CauldronLiquidType liquid_type) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    const auto &block = region.getBlock(pos);
    if (block.getName() != VanillaBlockTypeIds::Cauldron) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    const auto level = std::clamp(liquid_level, MIN_FILL_LEVEL, MAX_FILL_LEVEL);
    const auto &filled_block = *block.setState<int>(fill_level, level);
    const auto &new_block = *filled_block.setState<int>(cauldron_liquid, static_cast<int>(liquid_type));
    if (new_block.getRuntimeId() == block.getRuntimeId()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    endstone::Nullable<endstone::Actor> actor;
    auto reason = endstone::CauldronLevelChangeEvent::ChangeReason::Unknown;
    if (auto level_entity = region.getLevel().getLevelEntity().unwrap(); level_entity) {
        const auto *interaction =
            level_entity->tryGetComponent<endstone::core::InternalCauldronInteractionComponent>();
        if (interaction && interaction->pos == pos) {
            actor = interaction->actor;
            reason = interaction->reason;
        }
    }

    auto block_handle = endstone::core::EndstoneBlock::at(region, pos);
    auto new_state = block_handle->captureState(true);
    new_state->setData(endstone::core::EndstoneBlockData(const_cast<::Block &>(new_block)));
    endstone::CauldronLevelChangeEvent event{
        block_handle,
        actor,
        reason,
        new_state,
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return;
    }

    const auto &final_block =
        static_cast<const endstone::core::EndstoneBlockData &>(*event.getNewState()->getData()).getHandle();
    if (final_block.getName() != VanillaBlockTypeIds::Cauldron) {
        event.getNewState()->update(true);
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos,
                                final_block.getState<int>(fill_level),
                                static_cast<CauldronLiquidType>(final_block.getState<int>(cauldron_liquid)));
}
