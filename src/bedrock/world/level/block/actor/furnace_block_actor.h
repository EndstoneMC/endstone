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

#include <unordered_set>

#include "bedrock/bedrock.h"
#include "bedrock/core/string/string_hash.h"
#include "bedrock/shared_types/legacy/level_sound_event.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

class FurnaceBlockActor : public VanillaBlockActor, public Container {
public:
    static constexpr BlockActorType TypeId = BlockActorType::Furnace;

    explicit FurnaceBlockActor(const BlockPos &pos);

    static float getBurnDuration(class ItemStackBase const &item_instance, float burn_interval);

    [[nodiscard]] int getLitTime() const { return lit_time_; }
    void setLitTime(int value) { lit_time_ = value; }
    [[nodiscard]] int getLitDuration() const { return lit_duration_; }
    void setLitDuration(int value) { lit_duration_ = value; }
    [[nodiscard]] int getCookingProgress() const { return cooking_progress_; }
    void setCookingProgress(int value) { cooking_progress_ = value; }
    [[nodiscard]] int getStoredXP() const { return static_cast<int>(stored_xp_); }
    void setStoredXP(int value) { stored_xp_ = static_cast<unsigned int>(value); }
    [[nodiscard]] const ItemInstance &getLastFuelItem() const { return last_fuel_item_; }

private:
    enum : unsigned int {
        SLOT_INGREDIENT = 0,
        SLOT_FUEL = 1,
        SLOT_RESULT = 2,
        NUM_ITEMS = 3,
    };

    int lit_time_;
    int lit_duration_;
    int cooking_progress_;
    unsigned int stored_xp_;
    ItemStack items_[NUM_ITEMS];
    bool dirty_[NUM_ITEMS];
    std::unordered_set<ActorUniqueID> actors_;
    const HashedString recipe_tag_;
    const int burn_interval_;
    bool open_by_local_player_;
    SharedTypes::Legacy::LevelSoundEvent smelt_sound_event_;
    int sound_tick_;
    int sound_tick_target_;
    const ::Block &unlit_furnace_;
    const ::Block &lit_furnace_;
    ItemInstance last_fuel_item_;
    bool no_drop_;
    bool needs_lit_state_fixup_;
};
BEDROCK_STATIC_ASSERT_SIZE(FurnaceBlockActor, 1416, 1272);
