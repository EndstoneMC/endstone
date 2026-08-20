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

#include "endstone/runtime/bedrock_hooks/actor_interaction.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block_source.h"

namespace endstone::runtime {

EquipmentSlot getInteractionHand(const ::Player &player, const ::ItemStack &item)
{
    const auto matches_main_hand = item == player.getCarriedItem();
    const auto matches_off_hand = item == player.getOffhandSlot();
    return matches_off_hand && !matches_main_hand ? EquipmentSlot::OffHand : EquipmentSlot::Hand;
}

bool canBuild(::BlockSource &block_source, ::Actor &actor, const ::BlockPos &position, FacingID face,
              const ::ItemStackBase &item_stack)
{
    return block_source.checkBlockPermissions(actor, position, face, item_stack, false);
}

}  // namespace endstone::runtime
