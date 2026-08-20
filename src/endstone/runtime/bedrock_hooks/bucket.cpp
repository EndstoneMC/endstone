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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "bedrock/world/level/block_pos.h"
#include "endstone/runtime/bedrock_hooks/bucket_empty.h"
#include "endstone/runtime/bedrock_hooks/bucket_fill.h"
#include "endstone/runtime/vtable_hook.h"

class BucketItem : public Item {
public:
    [[nodiscard]] BucketFillType getFillType() const { return fill_type_; }
    InteractionResult useOn(ItemStack &, Actor &, BlockPos, FacingID, Vec3 const &) const;

private:
    BucketFillType fill_type_;
};

BucketFillType endstone::runtime::getBucketFillType(const ::Item &item)
{
    return item.isBucket() ? static_cast<const ::BucketItem &>(item).getFillType() : BucketFillType::Unknown;
}

InteractionResult BucketItem::useOn(::ItemStack &item_stack, ::Actor &actor, BlockPos position, FacingID face,
                                    const Vec3 &click_pos) const
{
    const auto action = endstone::runtime::handleBucketEmptyEvent(actor, position, face, item_stack);
    if (action == endstone::runtime::BucketEmptyAction::Cancel) {
        return InteractionResult::Failure();
    }
    if (action == endstone::runtime::BucketEmptyAction::Consume) {
        return InteractionResult::Success();
    }

    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&BucketItem::useOn, this, item_stack, actor, position, face,
                                                     click_pos);
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
