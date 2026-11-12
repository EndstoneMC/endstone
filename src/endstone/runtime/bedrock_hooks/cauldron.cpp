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

#include <algorithm>
#include <memory>
#include <optional>
#include <string_view>

#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/bucket_fill_type.h"
#include "bedrock/world/item/bucket_item_layout.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/actor/cauldron_block_actor.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/cauldron_block.h"
#include "bedrock/world/level/block/vanilla_block_type_ids.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/material/material.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/level/level.h"
#include "endstone/core/player.h"
#include "endstone/core/plugin/plugin_manager.h"
#include "endstone/core/server.h"
#include "endstone/event/block/cauldron_level_change_event.h"
#include "endstone/event/player/player_bucket_empty_event.h"
#include "endstone/event/player/player_bucket_fill_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/runtime/hook.h"
#include "endstone/runtime/item_utils.h"

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    using ChangeReason = endstone::CauldronLevelChangeEvent::ChangeReason;
    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};

    auto &player = event_data.player;
    auto &region = player.getDimensionBlockSource();
    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto &item_stack = player.getInventory().getItem(player.getSelectedItemSlot());
    const auto *item = item_stack.getItem();
    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(event_data.face);
    const auto &block = region.getBlock(event_data.pos);
    const auto *block_actor = static_cast<const CauldronBlockActor *>(region.getBlockEntity(event_data.pos));

    auto reason = ChangeReason::Unknown;
    auto decided = false;
    auto filling = false;
    std::optional<int> new_level;
    auto new_liquid = CauldronLiquidType::Water;
    std::optional<::ItemStack> result_item;
    if (block_actor && item && !item_stack.isNull()) {
        const auto level = block.getState<int>(fill_level);
        const auto liquid = static_cast<CauldronLiquidType>(block.getState<int>(cauldron_liquid));
        const auto potion = level > 0 && block_actor->getPotionId() >= 0;
        const auto water = level > 0 && !potion && liquid == CauldronLiquidType::Water;
        const auto clean = water && !block_actor->hasCustomColor();
        const auto above = event_data.pos.above();
        const std::string_view name = item->getFullItemName();
        const auto aux = item_stack.getAuxValue();
        if (region.getLiquidBlock(above).getMaterial().isType(MaterialType::Water) &&
            !region.getBlock(above).getBlockType().hasProperty(BlockProperty::Trapdoor)) {
            decided = true;
            if (level < MAX_FILL_LEVEL || !clean) {
                new_level = MAX_FILL_LEVEL;
            }
        }
        else if (item->isBucket()) {
            const auto fill_type = static_cast<const BucketItemLayout &>(*item).getFillType();
            filling = fill_type == BucketFillType::Empty;
            if (filling) {
                if (level == MAX_FILL_LEVEL && !potion) {
                    decided = true;
                    reason = ChangeReason::BucketFill;
                    new_level = MIN_FILL_LEVEL;
                    switch (liquid) {
                    case CauldronLiquidType::Water:
                        result_item = ::ItemStack("minecraft:water_bucket");
                        break;
                    case CauldronLiquidType::Lava:
                        result_item = ::ItemStack("minecraft:lava_bucket");
                        break;
                    default:
                        result_item = ::ItemStack("minecraft:powder_snow_bucket");
                        break;
                    }
                }
            }
            else if (fill_type == BucketFillType::Water || fill_type == BucketFillType::Lava ||
                     fill_type == BucketFillType::PowderSnow) {
                decided = true;
                reason = ChangeReason::BucketEmpty;
                result_item = ::ItemStack("minecraft:bucket");
                auto bucket_liquid = CauldronLiquidType::PowderSnow;
                if (fill_type == BucketFillType::Water) {
                    bucket_liquid = CauldronLiquidType::Water;
                }
                else if (fill_type == BucketFillType::Lava) {
                    bucket_liquid = CauldronLiquidType::Lava;
                }
                if (level == MIN_FILL_LEVEL ||
                    (bucket_liquid == CauldronLiquidType::Water ? water : liquid == bucket_liquid)) {
                    new_level = MAX_FILL_LEVEL;
                    new_liquid = bucket_liquid;
                }
                else {
                    new_level = MIN_FILL_LEVEL;
                }
            }
        }
        else if (name == "minecraft:glass_bottle") {
            if (level >= 2 && (water || potion)) {
                decided = true;
                reason = ChangeReason::BottleFill;
                new_level = level - 2;
            }
        }
        else if (name == "minecraft:arrow" && aux == 0) {
            if (potion && block_actor->getPotionId() >= 5) {
                decided = true;
                const auto used = std::min(16 * (std::max(level, 3) - 2), static_cast<int>(item_stack.getCount()));
                const auto remaining = level - (used + 15) / 16;
                new_level = remaining >= 3 ? remaining : MIN_FILL_LEVEL;
            }
        }
        else if (name == "minecraft:potion" || name == "minecraft:splash_potion" ||
                 name == "minecraft:lingering_potion") {
            const auto same = potion && block_actor->getPotionId() == aux;
            if (aux == 0) {
                decided = true;
                new_level = level == MIN_FILL_LEVEL || water ? std::min(level + 2, MAX_FILL_LEVEL) : MIN_FILL_LEVEL;
            }
            else if (aux > 4) {
                if (level == MIN_FILL_LEVEL || (same && level < MAX_FILL_LEVEL)) {
                    decided = true;
                    new_level = std::min(level + 2, MAX_FILL_LEVEL);
                }
                else if (!same) {
                    decided = true;
                    new_level = MIN_FILL_LEVEL;
                }
            }
            if (decided) {
                reason = ChangeReason::BottleEmpty;
            }
        }
        else if (item->isDyeable()) {
            if (water && (!clean || item->hasCustomColor(item_stack))) {
                decided = true;
                reason = clean ? ChangeReason::ArmorWash : ChangeReason::Unknown;
                new_level = level - 1;
            }
        }
        else if (water) {
            if (item->isHumanoidArmor() || name.ends_with("horse_armor") || name == "minecraft:wolf_armor") {
                if (!clean || item->hasCustomColor(item_stack)) {
                    decided = true;
                    reason = clean ? ChangeReason::ArmorWash : ChangeReason::Unknown;
                    new_level = level - 1;
                }
            }
            else if (clean && name.ends_with("shulker_box") && name != "minecraft:undyed_shulker_box") {
                decided = true;
                reason = ChangeReason::ShulkerWash;
                new_level = level - 1;
            }
            else if (item->isDye()) {
                decided = true;
                new_level = level;
            }
            else if (name == "minecraft:banner") {
                const auto *tag = item_stack.getUserData();
                const auto *patterns = tag ? tag->getList("Patterns") : nullptr;
                if (tag && tag->getInt("Type") == 0 && patterns && patterns->size() > 0) {
                    decided = true;
                    reason = ChangeReason::BannerWash;
                    new_level = level - 1;
                }
            }
        }
    }

    const ::Block *new_block = nullptr;
    if (new_level) {
        const auto &leveled = *block.setState<int>(fill_level, std::clamp(*new_level, MIN_FILL_LEVEL, MAX_FILL_LEVEL));
        new_block = &*leveled.setState<int>(cauldron_liquid, static_cast<int>(new_liquid));
    }

    std::optional<endstone::ItemStack> replacement;
    if (result_item && block_face) {
        auto block_handle = endstone::core::EndstoneBlock::at(region, event_data.pos);
        auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item_stack);
        auto cancelled = false;
        if (filling) {
            const auto event = std::make_shared<endstone::PlayerBucketFillEvent>(
                player.getEndstoneActor<endstone::core::EndstonePlayer>(), block_handle, block_handle, *block_face,
                bucket_stack.getType(), endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
                endstone::EquipmentSlot::Hand);
            server.getPluginManager().callEvent(event);
            cancelled = event->isCancelled();
            replacement = event->getItemStack();
        }
        else {
            const auto event = std::make_shared<endstone::PlayerBucketEmptyEvent>(
                player.getEndstoneActor<endstone::core::EndstonePlayer>(), block_handle, block_handle, *block_face,
                bucket_stack.getType(), endstone::core::EndstoneItemStack::fromMinecraft(*result_item),
                endstone::EquipmentSlot::Hand);
            server.getPluginManager().callEvent(event);
            cancelled = event->isCancelled();
            replacement = event->getItemStack();
        }
        if (cancelled) {
            return;
        }
    }

    std::shared_ptr<endstone::CauldronLevelChangeEvent> level_event;
    if (new_block && server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        auto block_handle = endstone::core::EndstoneBlock::at(region, event_data.pos);
        auto new_state = block_handle->captureState(true);
        new_state->setData(endstone::core::EndstoneBlockData(const_cast<::Block &>(*new_block)));
        level_event = std::make_shared<endstone::CauldronLevelChangeEvent>(
            block_handle, player.getEndstoneActor<endstone::core::EndstonePlayer>(), reason, new_state);
        server.getPluginManager().callEvent(level_event);
        if (level_event->isCancelled()) {
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
        interaction.handled = decided;
        interaction.has_result = result_item.has_value();
        interaction.result = replacement;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
    if (level_event && event_data.successful.value_or(false)) {
        const auto &final_block =
            static_cast<const endstone::core::EndstoneBlockData &>(*level_event->getNewState()->getData()).getHandle();
        if (final_block.getRuntimeId() != new_block->getRuntimeId()) {
            if (final_block.getName() != VanillaBlockTypeIds::Cauldron) {
                level_event->getNewState()->update(true);
            }
            else if (final_block.getRuntimeId() != region.getBlock(event_data.pos).getRuntimeId()) {
                setLiquidLevel(region, event_data.pos, final_block.getState<int>(fill_level),
                               static_cast<CauldronLiquidType>(final_block.getState<int>(cauldron_liquid)));
            }
        }
    }
    if (level_entity) {
        level_entity->removeComponent<endstone::core::InternalCauldronInteractionComponent>();
    }
}

void CauldronBlock::tick(BlockEvents::BlockQueuedTickEvent &event_data) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto level_entity = server.getEndstoneLevel()->getHandle().getLevelEntity().unwrap();
    if (!level_entity || !server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::tick, this, event_data);
        return;
    }

    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    const auto &region = event_data.getBlockSource();
    const auto *block_actor = static_cast<const CauldronBlockActor *>(region.getBlockEntity(event_data.pos));
    const auto &block = event_data.getBlock();
    const auto level = block.getState<int>(fill_level);
    const auto potion = block_actor && level > 0 && block_actor->getPotionId() >= 0;
    const auto dyed = block_actor && level > 0 && !potion && block.getState<int>(cauldron_liquid) == 0 &&
                      block_actor->hasCustomColor();
    const auto above = event_data.pos.above();
    const auto refill = block_actor && region.getLiquidBlock(above).getMaterial().isType(MaterialType::Water) &&
                        !region.getBlock(above).getBlockType().hasProperty(BlockProperty::Trapdoor) &&
                        (level == MIN_FILL_LEVEL || potion || dyed);
    const auto potion_id = block_actor ? block_actor->getPotionId() : -1;
    const auto potion_type = block_actor ? block_actor->getPotionType() : PotionType{};
    const auto has_color = block_actor && block_actor->hasCustomColor();
    const auto color = block_actor ? block_actor->getCustomColor() : 0;

    auto &interaction = level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>();
    interaction.reason = endstone::CauldronLevelChangeEvent::ChangeReason::NaturalFill;
    interaction.any_pos = true;
    interaction.refill = refill;
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::tick, this, event_data);
    if (refill && level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>().cancelled) {
        auto *restored = const_cast<CauldronBlockActor *>(block_actor);
        restored->setPotionId(potion_id);
        restored->setPotionType(potion_type);
        restored->setCustomColor(has_color, color);
    }
    level_entity->removeComponent<endstone::core::InternalCauldronInteractionComponent>();
}

void CauldronBlock::handlePrecipitation(BlockSource &region, const BlockPos &pos, float temperature,
                                        float rain_intensity) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto level_entity = region.getLevel().getLevelEntity().unwrap();
    if (!level_entity || !server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::handlePrecipitation, this, region, pos, temperature,
                                    rain_intensity);
        return;
    }

    auto &interaction = level_entity->getOrAddComponent<endstone::core::InternalCauldronInteractionComponent>();
    interaction.reason = endstone::CauldronLevelChangeEvent::ChangeReason::NaturalFill;
    interaction.any_pos = true;
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::handlePrecipitation, this, region, pos, temperature, rain_intensity);
    level_entity->removeComponent<endstone::core::InternalCauldronInteractionComponent>();
}

void CauldronBlock::_useInventory(::Player &player, ::ItemStack &current, ::ItemStack &replace_with, int use_count)
{
    auto level_entity = player.getLevel().getLevelEntity().unwrap();
    const auto *interaction =
        level_entity ? level_entity->tryGetComponent<endstone::core::InternalCauldronInteractionComponent>() : nullptr;
    if (!interaction || !interaction->has_result) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::_useInventory, player, current, replace_with, use_count);
        return;
    }

    auto filled =
        interaction->result ? endstone::core::EndstoneItemStack::toMinecraft(*interaction->result) : ::ItemStack{};
    if (player.getAbilities().getBool(AbilitiesIndex::Instabuild) &&
        endstone::runtime::inventoryContains(player, filled)) {
        filled = ::ItemStack{};
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::_useInventory, player, current, filled, use_count);
}

void CauldronBlock::setLiquidLevel(BlockSource &region, const BlockPos &pos, int liquid_level,
                                   CauldronLiquidType liquid_type) const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    if (!server.getEndstonePluginManager().isEventRegistered<endstone::CauldronLevelChangeEvent>()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    endstone::Nullable<endstone::Actor> actor;
    auto reason = endstone::CauldronLevelChangeEvent::ChangeReason::Unknown;
    endstone::core::InternalCauldronInteractionComponent *interaction = nullptr;
    if (auto level_entity = region.getLevel().getLevelEntity().unwrap(); level_entity) {
        interaction = level_entity->tryGetComponent<endstone::core::InternalCauldronInteractionComponent>();
        if (interaction && (interaction->any_pos || interaction->pos == pos)) {
            if (interaction->handled) {
                ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level,
                                            liquid_type);
                return;
            }
            if (interaction->refill) {
                if (interaction->cancelled || interaction->skip) {
                    return;
                }
                if (liquid_level == MAX_FILL_LEVEL) {
                    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level,
                                                liquid_type);
                    return;
                }
                liquid_level = MAX_FILL_LEVEL;
            }
            actor = interaction->source ? interaction->source->getEndstoneActor<endstone::Actor>() : interaction->actor;
            reason = interaction->reason;
        }
        else {
            interaction = nullptr;
        }
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
    if (new_block.getRuntimeId() == block.getRuntimeId() && !(interaction && interaction->refill)) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos, liquid_level, liquid_type);
        return;
    }

    auto block_handle = endstone::core::EndstoneBlock::at(region, pos);
    auto new_state = block_handle->captureState(true);
    new_state->setData(endstone::core::EndstoneBlockData(const_cast<::Block &>(new_block)));
    const auto event = std::make_shared<endstone::CauldronLevelChangeEvent>(block_handle, actor, reason, new_state);
    server.getPluginManager().callEvent(event);
    if (event->isCancelled()) {
        if (interaction) {
            interaction->cancelled = true;
        }
        return;
    }

    const auto &final_block =
        static_cast<const endstone::core::EndstoneBlockData &>(*event->getNewState()->getData()).getHandle();
    if (interaction && interaction->refill && final_block.getRuntimeId() != new_block.getRuntimeId()) {
        interaction->skip = true;
    }
    if (final_block.getName() != VanillaBlockTypeIds::Cauldron) {
        event->getNewState()->update(true);
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, pos,
                                final_block.getState<int>(fill_level),
                                static_cast<CauldronLiquidType>(final_block.getState<int>(cauldron_liquid)));
}
