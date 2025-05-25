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

#include "bedrock/world/actor/player/player_inventory_slot_data.h"
#include "bedrock/world/item/item_stack.h"

struct PlayerItemInUse {
    static constexpr int ITEM_USE_INTERVAL_DURATION = 4;
    static constexpr int ITEM_USE_INTERVAL_COUNT = 6;
    static constexpr int MAX_ITEM_USE_DURATION = 25;
    [[nodiscard]] const ItemStack &getItemInUse() const;
    [[nodiscard]] int getDuration(const EntityContext &) const;
    [[nodiscard]] int getUsedDuration(const EntityContext &) const;
    [[nodiscard]] int getMoveToMouthDuration() const;
    [[nodiscard]] bool isInSlot(PlayerInventorySlotData) const;
    void setItemInUse(const ItemStack &, EntityContext &, int, PlayerInventorySlotData);
    void clearItemInUse(EntityContext &entity);
    void releaseUsing(Player &);
    bool shouldDisplayUseParticles(const EntityContext &);
    int duration;
    bool should_send_interaction_game_events;

private:
    ItemStack item_;
    PlayerInventorySlotData slot_;
};
static_assert(sizeof(PlayerItemInUse) == 168);
