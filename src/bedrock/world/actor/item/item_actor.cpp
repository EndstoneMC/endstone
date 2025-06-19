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

#include "bedrock/world/actor/item/item_actor.h"

const ItemStack &ItemActor::getItemStack() const
{
    return item_;
}

ItemStack &ItemActor::getItemStack()
{
    return item_;
}

void ItemActor::setNoPickUpDelay()
{
    pickup_delay_ = 0;
}

void ItemActor::setNeverPickUp()
{
    setPickUpDelay(INFINITE_PICKUP_DELAY);
}

void ItemActor::setPickUpDelay(int delay)
{
    pickup_delay_ = delay;
}

ItemActor *ItemActor::tryGetFromEntity(EntityContext &entity, bool include_removed)
{
    auto *actor = Actor::tryGetFromEntity(entity, include_removed);
    if (!actor) {
        return nullptr;
    }
    if (!actor->isType(ActorType::ItemEntity)) {
        return nullptr;
    }
    return static_cast<ItemActor *>(actor);
}

int ItemActor::getPickUpDelay() const
{
    return pickup_delay_;
}
