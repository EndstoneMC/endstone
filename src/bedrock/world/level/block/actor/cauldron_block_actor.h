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
#include "bedrock/forward.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

class CauldronBlockActor : public VanillaBlockActor, public Container {
public:
    static constexpr BlockActorType TypeId = BlockActorType::Cauldron;
    static constexpr int NUM_ITEMS = 10;

    explicit CauldronBlockActor(const BlockPos &pos);

    // Endstone
    [[nodiscard]] int getPotionId() const { return potion_id_; }
    void setPotionId(int value) { potion_id_ = value; }
    [[nodiscard]] PotionType getPotionType() const { return potion_type_; }
    void setPotionType(PotionType value) { potion_type_ = value; }
    [[nodiscard]] bool hasCustomColor() const { return has_custom_color_; }
    [[nodiscard]] int getCustomColor() const { return custom_color_; }
    void setCustomColor(bool has_custom_color, int custom_color)
    {
        has_custom_color_ = has_custom_color;
        custom_color_ = custom_color;
    }

private:
    int potion_id_;
    PotionType potion_type_;
    bool has_custom_color_;
    int custom_color_;
    ItemStack items_[NUM_ITEMS];
};
BEDROCK_STATIC_ASSERT_SIZE(CauldronBlockActor, 2192, 2080);
