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

#include "bedrock/world/actor/item/experience_orb.h"

#include "bedrock/world/actor/player/player.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/runtime/hook.h"

namespace {
thread_local const ExperienceOrb *touched_orb = nullptr;
thread_local Player *touching_player = nullptr;
}  // namespace

void ExperienceOrb::playerTouch(Player &player)
{
    touched_orb = this;
    touching_player = &player;
    ENDSTONE_HOOK_CALL_ORIGINAL(&ExperienceOrb::playerTouch, this, player);
    touched_orb = nullptr;
    touching_player = nullptr;
    player.addOrRemoveComponent<endstone::core::InternalExperienceOrbPickupCancelledFlagComponent>(false);
}

void Actor::remove()
{
    if (this == touched_orb &&
        touching_player->hasComponent<endstone::core::InternalExperienceOrbPickupCancelledFlagComponent>()) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&Actor::remove, this);
}
