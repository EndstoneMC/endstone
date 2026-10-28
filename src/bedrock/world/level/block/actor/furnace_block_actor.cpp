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

#include "bedrock/world/level/block/actor/furnace_block_actor.h"

#include <algorithm>
#include <cstddef>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/symbol.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack_base.h"

float FurnaceBlockActor::getBurnDuration(ItemStackBase const &item_instance, float burn_interval)
{
    if (!item_instance) {
        return 0.0F;
    }

    const auto *item = item_instance.getItem();
    if (item->isComponentBased()) {
        static const HashedString fuel_identifier("minecraft:fuel");
        if (const auto *fuel = item->getComponent(fuel_identifier)) {
            const auto duration = *reinterpret_cast<const float *>(reinterpret_cast<const std::byte *>(fuel) + 16);
            return std::max(1.0F, burn_interval * duration / 10.0F);
        }
    }
    return getItemBurnDuration(*item, burn_interval);
}

float FurnaceBlockActor::getItemBurnDuration(Item const &item, float burn_interval)
{
    return BEDROCK_CALL(&FurnaceBlockActor::getItemBurnDuration, item, burn_interval);
}
