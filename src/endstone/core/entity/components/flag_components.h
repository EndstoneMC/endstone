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

#pragma once

#include <memory>
#include <optional>
#include <variant>

#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block_pos.h"
#include "endstone/event/block/cauldron_level_change_event.h"
#include "endstone/event/player/player_open_sign_event.h"
#include "endstone/inventory/inventory.h"
#include "endstone/inventory/item_stack.h"

class Actor;

namespace endstone::core {
struct InternalBucketFillActorComponent {
    const ::Actor *target = nullptr;
    std::optional<ItemStack> item_stack;
    bool write_item_stack = false;
};
struct InternalChangedDimensionFlagComponent {};
struct InternalCauldronInteractionComponent {
    Nullable<Actor> actor;
    ::Actor *source = nullptr;
    BlockPos pos;
    CauldronLevelChangeEvent::ChangeReason reason = CauldronLevelChangeEvent::ChangeReason::Unknown;
    bool any_pos = false;
    bool cancelled = false;
    bool refill = false;
    bool skip = false;
    bool handled = false;
    bool has_result = false;
    std::optional<ItemStack> result;
};
struct InternalCraftResultReplacedFlagComponent {};
struct InternalDisconnectFlagComponent {};
struct InternalEntityContainerOpenFlagComponent {};
struct InternalExperienceOrbPickupCancelledFlagComponent {};
struct InternalExperienceOrbPickupFlagComponent {};
struct InternalLoginCancelledFlagComponent {};
struct InternalOpenContainerComponent {
    std::variant<BlockPos, ActorUniqueID> owner;
    std::shared_ptr<Inventory> inventory;
};
struct InternalOpenSignCauseComponent {
    PlayerOpenSignEvent::Cause cause = PlayerOpenSignEvent::Cause::Unknown;
};
struct InternalRemoveFlagComponent {};
struct InternalSpawnPointCommandFlagComponent {};
struct InternalSuppressEnchantOptionsFlagComponent {};
struct InternalTeleportFlagComponent {};
struct MobHurtFlagComponent {};
}  // namespace endstone::core
