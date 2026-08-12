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

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/container.h"
#include "bedrock/world/level/block/cauldron_block.h"
#include "endstone/runtime/bedrock_hooks/bucket_empty.h"
#include "endstone/runtime/hook.h"

void CauldronBlock::use(BlockEvents::BlockPlayerInteractEvent &event_data) const
{
    auto &player = event_data.player;
    const auto slot = player.getSelectedItemSlot();
    const auto &item_stack = player.getInventory().getItem(slot);
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
    ENDSTONE_HOOK_CALL_ORIGINAL(&CauldronBlock::use, this, event_data);
    endstone::runtime::handleCauldronBucketEmptyResult(player, event_data.pos, event_data.successful.value_or(false));
}
