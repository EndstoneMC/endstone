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

#include "bedrock/world/actor/player/player_item_in_use.h"

#include "bedrock/entity/components/item_in_use_component.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/vanilla_item_tags.h"

const ItemStack &PlayerItemInUse::getItemInUse() const
{
    return item_;
}

int PlayerItemInUse::getDuration(const EntityContext &entity) const
{
    const auto *item_in_use = entity.tryGetComponent<ItemInUseComponent>();
    if (!item_in_use) {
        return 0;
    }
    return item_in_use->duration;
}

void PlayerItemInUse::setItemInUse(const ItemStack &new_item, EntityContext &owner, int duration,
                                   PlayerInventorySlotData slot)
{
    item_ = new_item;
    should_send_interaction_game_events = item_.getItem()->shouldSendInteractionGameEvents();
    slot_ = slot;
    auto &item_in_use = owner.getOrAddComponent<ItemInUseComponent>();
    item_in_use.item = item_.getItemPtr();
    item_in_use.duration = duration;
    if (item_.hasTag(VanillaItemTags::Trident)) {
        owner.getOrAddComponent<ItemInUseTicksDuringMovementComponent>();
    }
}

void PlayerItemInUse::clearItemInUse(EntityContext &entity)
{
    item_.setNull(std::nullopt);
    entity.removeComponent<ItemInUseComponent>();
    entity.removeComponent<ItemInUseTicksDuringMovementComponent>();
    slot_ = {-1, CONTAINER_ID_NONE};
}
