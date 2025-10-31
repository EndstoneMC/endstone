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

#include "endstone/core/actor/actor.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/provider/actor_offset.h"
#include "endstone/core/actor/item.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/player.h"

endstone::core::EndstoneActor &Actor::getEndstoneActor0() const
{
    return *getEndstoneActorPtr();
}

std::shared_ptr<endstone::core::EndstoneActor> Actor::getEndstoneActorPtr0() const
{
    auto *self = const_cast<Actor *>(this);
    auto &component = entity_context_.getOrAddComponent<endstone::core::EndstoneActorComponent>();
    if (component.actor) {
        return component.actor;
    }

    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    if (auto *player = Player::tryGetFromEntity(self->entity_context_); player) {
        component.actor = std::make_shared<endstone::core::EndstonePlayer>(server, *player);
        component.actor->recalculatePermissions();
    }
    else if (auto *mob = Mob::tryGetFromEntity(self->entity_context_); mob) {
        component.actor = std::make_shared<endstone::core::EndstoneMob>(server, *mob);
    }
    else if (auto *item = ItemActor::tryGetFromEntity(self->entity_context_); item) {
        component.actor = std::make_shared<endstone::core::EndstoneItem>(server, *item);
    }
    else {
        component.actor = std::make_shared<endstone::core::EndstoneActor>(server, *self);
    }
    return component.actor;
}
