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
#include "bedrock/world/attribute/attribute_instance.h"
#include "bedrock/world/attribute/attribute_instance_delegate.h"

class Player;

class HungerAttributeDelegate : public AttributeInstanceDelegate {
public:
    void tick(AttributeInstance &, AttributeModificationContext &) override;
    ENDSTONE_HOOK void notify(AttributeMessageType, AttributeModificationContext &) override;

    // Endstone begin
    [[nodiscard]] Player *getPlayer() const { return player_; }
    // Endstone end

private:
    int action_tick_timer_;
    int tick_counter_;
    float last_food_level_;
    Player *player_;
};
BEDROCK_STATIC_ASSERT_SIZE(HungerAttributeDelegate, 40, 32);
