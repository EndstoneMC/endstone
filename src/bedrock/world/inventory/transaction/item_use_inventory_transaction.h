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

#include <cstdint>

#include "bedrock/common_types.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/inventory/transaction/complex_inventory_transaction.h"
#include "bedrock/world/item/network_item_stack_descriptor.h"
#include "bedrock/world/level/block_pos.h"

class ItemUseInventoryTransaction : public ComplexInventoryTransaction {
public:
    enum class ActionType : int {
        Place = 0,
        Use = 1,
        Destroy = 2,
        UseAsAttack = 3,
    };
    enum class TriggerType : std::uint8_t {
        Unknown = 0,
        PlayerInput = 1,
        SimulationTick = 2,
    };
    enum class PredictedResult : std::uint8_t {
        Failure = 0,
        Success = 1,
    };
    enum class ClientCooldownState : std::uint8_t {
        Off = 0,
        On = 1,
    };

    ActionType action_type_;
    TriggerType trigger_type_;
    BlockPos pos_;
    BlockRuntimeId target_block_id_;
    FacingID face_;
    std::int32_t slot_;
    NetworkItemStackDescriptor item_;
    Vec3 from_pos_;
    Vec3 click_pos_;
    PredictedResult client_predicted_result_;
    ClientCooldownState client_cooldown_state_;
};
