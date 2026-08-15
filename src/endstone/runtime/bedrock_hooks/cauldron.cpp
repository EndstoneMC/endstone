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
#include <optional>
#include <utility>

#include "bedrock/entity/systems/server_stand_in_cauldron_system/system_impl.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/cauldron_block.h"
#include "bedrock/world/level/storage/game_rules.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/server.h"
#include "endstone/runtime/bedrock_hooks/bucket.h"
#include "endstone/event/actor/actor_inside_block_event.h"
#include "endstone/runtime/bedrock_hooks/bucket_empty.h"
#include "endstone/runtime/bedrock_hooks/bucket_fill.h"
#include "endstone/runtime/bedrock_hooks/cauldron.h"
#include "endstone/runtime/hook.h"

namespace {

using ChangeReason = endstone::CauldronLevelChangeEvent::ChangeReason;

using endstone::runtime::CauldronChangeContext;
using endstone::runtime::ScopedCauldronChangeContext;

thread_local CauldronChangeContext *cauldron_change_context;

constexpr int water_potion_aux_value = 30;

ChangeReason getChangeReason(int old_level, int new_level, int old_liquid, int new_liquid)
{
    if (new_level > old_level || (new_level == old_level && new_liquid != old_liquid)) {
        return ChangeReason::NaturalFill;
    }
    return ChangeReason::Unknown;
}

bool isFilledCauldron(const ::Block &block)
{
    static const HashedString fill_level{"fill_level"};
    return block.getName().getString() == "minecraft:cauldron" && block.getState<int>(fill_level) > 0;
}

bool isCauldron(const ::Block &block)
{
    return block.getName().getString() == "minecraft:cauldron";
}

const ::Block *makeCauldronState(const ::Block &block, int liquid_level, CauldronLiquidType liquid_type)
{
    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    const auto *new_block = block.setState<int>(fill_level, std::clamp(liquid_level, 0, 6)).get();
    return new_block->setState<int>(cauldron_liquid, static_cast<int>(liquid_type)).get();
}

int getCauldronLevel(const ::Block &block)
{
    static const HashedString fill_level{"fill_level"};
    return block.getState<int>(fill_level);
}

CauldronLiquidType getCauldronLiquid(const ::Block &block)
{
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    return static_cast<CauldronLiquidType>(block.getState<int>(cauldron_liquid));
}

bool isNonLavaCauldron(const ::Block &block)
{
    return isFilledCauldron(block) && getCauldronLiquid(block) != CauldronLiquidType::Lava;
}

bool isWaterPotion(const ::ItemStack &item_stack)
{
    return item_stack.getAuxValue() == water_potion_aux_value;
}

bool hasBannerPattern(const ::ItemStack &item_stack)
{
    const auto *user_data = item_stack.getUserData();
    const auto *patterns = user_data ? user_data->getList("Patterns") : nullptr;
    return patterns && patterns->size() > 0;
}

bool isDyedShulkerBox(const ::Item &item)
{
    const auto &name = item.getFullItemName();
    return name.ends_with("_shulker_box") && name != "minecraft:undyed_shulker_box";
}

const ::Block *getCauldronUseState(BlockSource &region, const BlockPos &position, const ::Block &block,
                                   const ::ItemStack &item_stack)
{
    if (!isCauldron(block)) {
        return nullptr;
    }

    const auto level = getCauldronLevel(block);
    const auto liquid = getCauldronLiquid(block);
    const auto *item = item_stack.getItem();
    if (!item) {
        return nullptr;
    }

    const auto reason = endstone::runtime::getCauldronChangeReason(item_stack);
    switch (reason) {
    case ChangeReason::BucketFill:
        return level == 6 ? makeCauldronState(block, 0, CauldronLiquidType::Water) : nullptr;
    case ChangeReason::BucketEmpty: {
        if (endstone::runtime::isWaterContentBucket(*item) &&
            (level == 0 || (liquid == CauldronLiquidType::Water && level < 6))) {
            return makeCauldronState(block, 6, CauldronLiquidType::Water);
        }
        const auto bucket_type = endstone::runtime::getBucketFillType(*item);
        if (level == 0 &&
            (bucket_type == BucketFillType::Lava || bucket_type == BucketFillType::PowderSnow) &&
            !region.isUnderWater(position, block)) {
            return makeCauldronState(
                block, 6,
                bucket_type == BucketFillType::Lava ? CauldronLiquidType::Lava : CauldronLiquidType::PowderSnow);
        }
        return nullptr;
    }
    case ChangeReason::BottleFill:
        return liquid == CauldronLiquidType::Water && level > 0
                   ? makeCauldronState(block, level - 2, CauldronLiquidType::Water)
                   : nullptr;
    case ChangeReason::BottleEmpty:
        if (!isWaterPotion(item_stack) || level == 6 || (level > 0 && liquid != CauldronLiquidType::Water)) {
            return nullptr;
        }
        return makeCauldronState(block, level + 2, CauldronLiquidType::Water);
    case ChangeReason::BannerWash:
    case ChangeReason::ShulkerWash:
    case ChangeReason::ArmorWash:
        if (liquid != CauldronLiquidType::Water || level == 0) {
            return nullptr;
        }
        if (reason == ChangeReason::BannerWash && !hasBannerPattern(item_stack)) {
            return nullptr;
        }
        if (reason == ChangeReason::ArmorWash && !item->hasCustomColor(item_stack)) {
            return nullptr;
        }
        return makeCauldronState(block, level - 1, CauldronLiquidType::Water);
    case ChangeReason::NaturalFill:
    case ChangeReason::Extinguish:
    case ChangeReason::Evaporate:
    case ChangeReason::Unknown:
        return nullptr;
    }
    return nullptr;
}

bool callCauldronLevelChangeEvent(BlockSource &region, const BlockPos &position, const ::Block &old_block,
                                  const ::Block &new_block, CauldronChangeContext *context, bool apply_new_state,
                                  bool apply_modified_state, bool *state_modified = nullptr)
{
    static const HashedString fill_level{"fill_level"};
    static const HashedString cauldron_liquid{"cauldron_liquid"};
    const auto old_level = old_block.getState<int>(fill_level);
    const auto old_liquid = old_block.getState<int>(cauldron_liquid);
    const auto new_level = new_block.getState<int>(fill_level);
    const auto new_liquid = new_block.getState<int>(cauldron_liquid);
    auto block_wrapper = endstone::core::EndstoneBlock::at(region, position);
    auto new_state = block_wrapper->captureState();
    new_state->setData(endstone::core::EndstoneBlockData(*const_cast<::Block *>(&new_block)));

    const auto reason = context && context->reason != ChangeReason::Unknown
                            ? context->reason
                            : getChangeReason(old_level, new_level, old_liquid, new_liquid);
    endstone::CauldronLevelChangeEvent event{
        std::move(block_wrapper),
        context && context->entity ? context->entity->getEndstoneActor<endstone::Actor>() : nullptr,
        reason,
        std::move(new_state),
    };
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        if (context) {
            context->cancelled = true;
        }
        return false;
    }

    const auto modified = event.getNewState().getData()->getRuntimeId() != new_block.getRuntimeId();
    if (state_modified) {
        *state_modified = modified;
    }
    if (context) {
        context->event_fired = true;
        if (context->defer_state_update) {
            context->deferred_data = event.getNewState().getData();
        }
    }
    if (apply_new_state || (apply_modified_state && modified)) {
        event.getNewState().update(true);
    }
    return true;
}

void applyDeferredCauldronState(CauldronChangeContext &context)
{
    if (!context.deferred_data || context.state_applied) {
        return;
    }
    endstone::core::EndstoneBlock::at(*context.region, context.position)
        ->setData(*context.deferred_data, true);
    context.state_applied = true;
}

bool prepareCauldronExtinguish(BlockSource &region, const BlockPos &position, CauldronChangeContext &context)
{
    const auto &block = region.getBlock(position);
    if (!isNonLavaCauldron(block)) {
        return true;
    }

    static const HashedString fill_level{"fill_level"};
    // BDS passes Water for this path, including PowderSnow cauldrons.
    const auto new_block = makeCauldronState(block, block.getState<int>(fill_level) - 1, CauldronLiquidType::Water);
    return callCauldronLevelChangeEvent(region, position, block, *new_block, &context, false, true,
                                        &context.skip_setter);
}
}  // namespace

endstone::runtime::ScopedCauldronChangeContext::ScopedCauldronChangeContext(
    endstone::runtime::CauldronChangeContext &context)
    : previous_(cauldron_change_context)
{
    cauldron_change_context = &context;
}

endstone::runtime::ScopedCauldronChangeContext::~ScopedCauldronChangeContext()
{
    cauldron_change_context = previous_;
}

endstone::runtime::CauldronChangeContext::CauldronChangeContext(
    ::Actor *entity, endstone::runtime::CauldronChangeReason reason, ::BlockSource &region, const BlockPos &position,
    const ::Block &original_block)
    : entity(entity), reason(reason), region(&region), position(position), original_block(&original_block)
{
}

endstone::runtime::CauldronChangeContext *endstone::runtime::getCauldronChangeContext()
{
    return cauldron_change_context;
}

bool endstone::runtime::finishCauldronChange(endstone::runtime::CauldronChangeContext &context)
{
    if (context.finalized) {
        return !context.cancelled;
    }
    context.finalized = true;
    auto &region = *context.region;
    if (context.cancelled) {
        if (region.getBlock(context.position).getRuntimeId() != context.original_block->getRuntimeId()) {
            region.setBlock(context.position, *context.original_block, ::BlockType::UPDATE_ALL, nullptr, nullptr);
        }
        return false;
    }
    if (context.event_fired) {
        applyDeferredCauldronState(context);
        return true;
    }

    const auto &current_block = region.getBlock(context.position);
    const auto *new_block = context.new_block ? context.new_block : &current_block;
    if (new_block->getRuntimeId() == context.original_block->getRuntimeId() ||
        !isCauldron(*context.original_block) || !isCauldron(*new_block)) {
        return true;
    }

    if (current_block.getRuntimeId() != context.original_block->getRuntimeId()) {
        region.setBlock(context.position, *context.original_block, ::BlockType::UPDATE_ALL, nullptr, nullptr);
    }
    return callCauldronLevelChangeEvent(region, context.position, *context.original_block, *new_block, &context, true,
                                        false);
}

endstone::runtime::CauldronChangeReason endstone::runtime::getCauldronChangeReason(
    const ::ItemStack &item_stack)
{
    const auto *item = item_stack.getItem();
    if (!item) {
        return CauldronChangeReason::Unknown;
    }

    const auto bucket_type = endstone::runtime::getBucketFillType(*item);
    if (bucket_type == BucketFillType::Empty) {
        return CauldronChangeReason::BucketFill;
    }
    if (item->isBucket() && bucket_type != BucketFillType::Milk) {
        return CauldronChangeReason::BucketEmpty;
    }
    const auto &name = item->getFullItemName();
    if (name == "minecraft:glass_bottle") {
        return CauldronChangeReason::BottleFill;
    }
    if (name == "minecraft:potion") {
        return CauldronChangeReason::BottleEmpty;
    }
    if (name == "minecraft:banner" || name.ends_with("_banner")) {
        return CauldronChangeReason::BannerWash;
    }
    if (isDyedShulkerBox(*item)) {
        return CauldronChangeReason::ShulkerWash;
    }
    if (item->isDyeable()) {
        return CauldronChangeReason::ArmorWash;
    }
    return CauldronChangeReason::Unknown;
}

bool endstone::runtime::prepareCauldronChange(CauldronChangeContext &context, const ::ItemStack &item_stack)
{
    const auto *new_block = getCauldronUseState(*context.region, context.position, *context.original_block, item_stack);
    if (!new_block) {
        return false;
    }

    context.defer_state_update = true;
    if (callCauldronLevelChangeEvent(*context.region, context.position, *context.original_block, *new_block, &context,
                                     false, false)) {
        context.skip_setter = true;
    }
    return true;
}

void CauldronBlock::setLiquidLevel(BlockSource &region, const BlockPos &position, int liquid_level,
                                   CauldronLiquidType liquid_type) const
{
    auto *context = cauldron_change_context;
    if (context && context->region == &region && context->position == position) {
        if (context->cancelled) {
            return;
        }
        if (context->event_fired) {
            if (context->defer_state_update) {
                applyDeferredCauldronState(*context);
                return;
            }
            if (!context->skip_setter) {
                ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, position, liquid_level,
                                            liquid_type);
            }
            return;
        }

        if (isCauldron(*context->original_block)) {
            const auto *new_block = makeCauldronState(*context->original_block, liquid_level, liquid_type);
            if (context->reason == ChangeReason::Unknown &&
                new_block->getRuntimeId() != context->original_block->getRuntimeId()) {
                context->defer_state_update = true;
                if (callCauldronLevelChangeEvent(region, position, *context->original_block, *new_block, context, false,
                                                 false)) {
                    applyDeferredCauldronState(*context);
                }
            }
            else {
                context->new_block = new_block;
            }
            return;
        }
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::setLiquidLevel, this, region, position, liquid_level, liquid_type);
}

void ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron(ActorOwnerComponent &actor_owner)
{
    auto &actor = actor_owner.getActor();
    auto &region = actor.getDimensionBlockSource();
    const auto &actor_position = actor.getPosition();
    const BlockPos position{actor_position.x, actor.getAABB().min.y, actor_position.z};
    const auto &block = region.getBlock(position);
    if (!isFilledCauldron(block)) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, actor_owner);
        return;
    }

    endstone::ActorInsideBlockEvent inside_event{actor.getEndstoneActor<endstone::Actor>(),
                                                 endstone::core::EndstoneBlock::at(region, position)};
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(inside_event);
    if (inside_event.isCancelled()) {
        return;
    }

    if (getCauldronLiquid(block) == CauldronLiquidType::Lava || !actor.isOnFire()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, actor_owner);
        return;
    }
    if (!actor.isPlayer() && !actor.getLevel().getGameRules().getBool(GameRuleId(GameRules::MOB_GRIEFING), false)) {
        return;
    }

    CauldronChangeContext context{&actor, ChangeReason::Extinguish, region, position, block};
    if (!prepareCauldronExtinguish(region, position, context)) {
        return;
    }
    ScopedCauldronChangeContext scope(context);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerStandInCauldronSystem::SystemImpl::_checkInsideCauldron, actor_owner);
}

void CauldronBlock::tick(BlockEvents::BlockQueuedTickEvent &event_data) const
{
    const auto &original_block = event_data.region.getBlock(event_data.pos);
    CauldronChangeContext context{nullptr, ChangeReason::Unknown, event_data.region, event_data.pos, original_block};
    const auto &block_above = event_data.region.getBlock(event_data.pos.above());
    if (getCauldronLevel(original_block) < 6 && getCauldronLiquid(original_block) == CauldronLiquidType::Water &&
        block_above.getMaterial().isType(MaterialType::Water) &&
        !block_above.hasProperty(BlockProperty::Trapdoor)) {
        const auto *new_block = makeCauldronState(original_block, 6, CauldronLiquidType::Water);
        context.reason = ChangeReason::NaturalFill;
        context.defer_state_update = true;
        if (!callCauldronLevelChangeEvent(event_data.region, event_data.pos, original_block, *new_block, &context,
                                          false, false)) {
            finishCauldronChange(context);
            return;
        }
        context.skip_setter = true;
    }
    {
        ScopedCauldronChangeContext scope(context);
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::tick, this, event_data);
    }
    finishCauldronChange(context);
}

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    auto &player = event_data.player;
    const auto slot = player.getSelectedItemSlot();
    const auto &item_stack = player.getInventory().getItem(slot);
    auto &block_source = player.getDimensionBlockSource();
    auto *parent = endstone::runtime::getCauldronChangeContext();
    const bool has_bucket_context =
        parent && parent->region == &block_source && parent->position == event_data.pos && parent->entity == &player &&
        (parent->reason == ChangeReason::BucketFill || parent->reason == ChangeReason::BucketEmpty);
    if (!has_bucket_context) {
        const auto action = endstone::runtime::handleBucketEmptyEvent(player, event_data.pos, event_data.face,
                                                                       item_stack);
        if (action == endstone::runtime::BucketEmptyAction::Cancel) {
            event_data.successful = false;
            return;
        }
        if (action == endstone::runtime::BucketEmptyAction::Consume) {
            event_data.successful = true;
            return;
        }
    }
    const auto &original_block = block_source.getBlock(event_data.pos);
    const bool owns_context = !parent || parent->region != &block_source || parent->position != event_data.pos ||
                              parent->entity != &player;
    std::optional<CauldronChangeContext> local_context;
    std::optional<ScopedCauldronChangeContext> local_scope;
    if (owns_context) {
        local_context.emplace(&player, endstone::runtime::getCauldronChangeReason(item_stack), block_source,
                              event_data.pos, original_block);
        parent = &*local_context;
    }
    if (!parent->event_fired && endstone::runtime::prepareCauldronChange(*parent, item_stack) && parent->cancelled) {
        event_data.successful = true;
    }
    if (!parent->cancelled) {
        if (owns_context) {
            local_scope.emplace(*parent);
        }
        ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
    }
    if (owns_context) {
        local_scope.reset();
        endstone::runtime::finishCauldronChange(*parent);
    }
    if (parent->cancelled) {
        endstone::runtime::cancelBucketEmptyResult();
        endstone::runtime::cancelBucketFillResult();
        event_data.successful = true;
    }
    endstone::runtime::handleCauldronBucketEmptyResult(player, event_data.pos,
                                                       event_data.successful.value_or(false) && !parent->cancelled);
}
