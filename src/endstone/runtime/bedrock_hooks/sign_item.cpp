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

#include "bedrock/world/item/sign_item.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/core/player_open_sign.h"
#include "endstone/runtime/hook.h"

InteractionResult SignItem::_useOn(ItemStack &item_stack, Actor &actor, BlockPos pos, FacingID face,
                                   Vec3 const &click_pos) const
{
    const auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&SignItem::_useOn, this, item_stack, actor, pos, face, click_pos);
    if (actor.isPlayer()) {
        endstone::core::clearPendingOpenSignCause(static_cast<::Player &>(actor));
    }
    return result;
}
