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

#include "endstone/runtime/bedrock_hooks/bucket.h"

#include <optional>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "endstone/runtime/bedrock_hooks/bucket_empty.h"
#include "endstone/runtime/bedrock_hooks/bucket_fill.h"
#include "endstone/runtime/bedrock_hooks/cauldron.h"
#include "endstone/runtime/vtable_hook.h"

BucketFillType endstone::runtime::getBucketFillType(const ::Item &item)
{
    return item.isBucket() ? static_cast<const ::BucketItem &>(item).getFillType() : BucketFillType::Unknown;
}

InteractionResult BucketItem::useOn(::ItemStack &item_stack, ::Actor &actor, BlockPos position, FacingID face,
                                    const Vec3 &click_pos) const
{
    auto &block_source = actor.getDimensionBlockSource();
    const auto &block = block_source.getBlock(position);
    const auto reason = endstone::runtime::getCauldronChangeReason(item_stack);
    std::optional<endstone::runtime::CauldronChangeContext> cauldron_context;
    std::optional<endstone::runtime::ScopedCauldronChangeContext> cauldron_scope;
    if (actor.isPlayer() && block.getName().getString() == "minecraft:cauldron" &&
        reason != endstone::runtime::CauldronChangeReason::Unknown) {
        cauldron_context.emplace(&actor, reason, block_source, position, block);
    }

    const auto action = endstone::runtime::handleBucketEmptyEvent(actor, position, face, item_stack);
    if (action == endstone::runtime::BucketEmptyAction::Cancel) {
        return InteractionResult::Failure();
    }
    if (action == endstone::runtime::BucketEmptyAction::Consume) {
        return InteractionResult::Success();
    }

    if (cauldron_context) {
        endstone::runtime::prepareCauldronChange(*cauldron_context, item_stack);
        if (cauldron_context->cancelled) {
            endstone::runtime::cancelBucketEmptyResult();
            endstone::runtime::cancelBucketFillResult();
            return InteractionResult::Success();
        }
        cauldron_scope.emplace(*cauldron_context);
    }

    const auto original_item_stack = item_stack;
    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&BucketItem::useOn, this, item_stack, actor, position, face,
                                                     click_pos);

    if (cauldron_context) {
        cauldron_scope.reset();
        endstone::runtime::finishCauldronChange(*cauldron_context);
    }

    if (cauldron_context && cauldron_context->cancelled) {
        endstone::runtime::cancelBucketEmptyResult();
        endstone::runtime::cancelBucketFillResult();
        item_stack.setUserData(nullptr);
        item_stack = original_item_stack;
        return InteractionResult::Success();
    }

    endstone::runtime::handleBucketFillResult(result, item_stack, actor, position);
    endstone::runtime::handleBucketEmptyResult(result, item_stack, actor, position);
    return result;
}

void endstone::runtime::installBucketHook()
{
    static bool installed = false;
    if (installed) {
        return;
    }

    const auto registry = ItemRegistryManager::getItemRegistry();
    if (!registry.isValid()) {
        return;
    }

    int aux_value = 0;
    const auto bucket = registry.lookupByName(aux_value, "minecraft:bucket");
    if (bucket.isNull()) {
        return;
    }

#ifdef _WIN32
    endstone::runtime::vhook::create<129>(bucket.get(), &BucketItem::useOn);
#else
    endstone::runtime::vhook::create<130>(bucket.get(), &BucketItem::useOn);
#endif
    installed = true;
}
