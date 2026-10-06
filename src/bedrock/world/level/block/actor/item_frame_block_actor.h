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

#include "bedrock/bedrock.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/world/item/clock_sprite_calculator.h"
#include "bedrock/world/item/compass_sprite_calculator.h"
#include "bedrock/world/item/item_instance.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

class ItemFrameBlockActor : public VanillaBlockActor {
public:
    static constexpr int NUM_ROTATIONS = 8;
    static constexpr float ROTATION_DEGREES = 45.0F;
    static constexpr BlockActorType TypeId = BlockActorType::ItemFrame;

    [[nodiscard]] const ItemInstance &getFramedItem() const { return item_; }
    [[nodiscard]] float getRotation() const { return rotation_; }
    void setItem(::BlockSource &region, const ItemInstance &item, ::Actor *entity_source);

    // Endstone
    [[nodiscard]] float getDropChance() const { return drop_chance_; }
    void setDropChance(float drop_chance) { drop_chance_ = drop_chance; }
    void setRotation(float rotation) { rotation_ = rotation; }

private:
    ClockSpriteCalculator clock_sprite_calc_;
    CompassSpriteCalculator compass_sprite_calc_;
    CompassSpriteCalculator recovery_compass_sprite_calc_;
    bool instant_clock_calculator_update_;
    ItemInstance item_;
    float drop_chance_;
    float rotation_;
    bool upgrade_map_bit_;
    bool upgrade_photo_bit_;
    bool refresh_map_;
    WeakEntityRef display_entity_;
    bool ignore_lighting_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemFrameBlockActor, 504, 480);
