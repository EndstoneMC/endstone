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

#include "endstone/core/actor/item.h"

#include <endstone/core/inventory/item_stack.h>

#include "endstone/core/server.h"

namespace endstone::core {

EndstoneItem::EndstoneItem(EndstoneServer &server, ::ItemActor &actor) : EndstoneActor(server, actor) {}

Item *EndstoneItem::asItem() const
{
    return const_cast<EndstoneItem *>(this);
}

std::unique_ptr<ItemStack> EndstoneItem::getItemStack() const
{
    return EndstoneItemStack::fromMinecraft(getItem().getItemStack());
}

void EndstoneItem::setItemStack(const ItemStack &stack)
{
    getItem().setItemStack(EndstoneItemStack::toMinecraft(&stack));
}

int EndstoneItem::getPickupDelay() const
{
    return getItem().getPickUpDelay();
}

void EndstoneItem::setPickupDelay(int delay)
{
    getItem().setPickUpDelay(delay);
}

void EndstoneItem::setUnlimitedLifetime(bool unlimited)
{
    getItem().setLifeTime(unlimited ? std::numeric_limits<int>::max() : ItemActor::LIFETIME);
}

bool EndstoneItem::isUnlimitedLifetime() const
{
    return getItem().getLifeTime() == std::numeric_limits<int>::max();
}

void EndstoneItem::setThrower(std::optional<std::int64_t> thrower)
{
    auto &data = getItem().entity_data;
    data.set(static_cast<SynchedActorData::ID>(ActorDataIDs::OWNER), thrower.value_or(0UL));
}

std::optional<std::int64_t> EndstoneItem::getThrower() const
{
    const auto &data = getItem().entity_data;
    if (data.hasData(static_cast<SynchedActorData::ID>(ActorDataIDs::OWNER))) {
        return data.getInt64(static_cast<SynchedActorData::ID>(ActorDataIDs::OWNER));
    }
    return std::nullopt;
}

ItemActor &EndstoneItem::getItem() const
{
    return getHandle<::ItemActor>();
}

}  // namespace endstone::core
