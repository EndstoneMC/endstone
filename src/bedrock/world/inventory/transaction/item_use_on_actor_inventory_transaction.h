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

#include "bedrock/bedrock.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/world/actor/actor_runtime_id.h"
#include "bedrock/world/inventory/transaction/complex_inventory_transaction.h"
#include "bedrock/world/item/network_item_stack_descriptor.h"

class Actor;

class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction {
public:
    enum class ActionType : int {
        Interact = 0,
        Attack = 1,
        ItemInteract = 2,
    };

    ActorRuntimeID runtime_id_;
    ActionType action_type_;
    std::int32_t slot_;
    NetworkItemStackDescriptor item_;
    Vec3 from_pos_;
    Vec3 hit_pos_;

    // The target function is the interaction callback created by handle().
    ENDSTONE_HOOK static void executeInteraction(void *context);

#ifdef _WIN32
    ENDSTONE_HOOK static std::int64_t executeBucketEntityUse(
        void *item_stack, void *result, Actor *entity, int arg3, int arg4, int arg5, char arg6, void *arg7,
        std::int64_t arg8);
#else
    ENDSTONE_HOOK static long long executeBucketEntityUse(
        void *item_stack, Actor *entity, int arg2, int arg3, int arg4, char arg5, void *arg6, long long arg7);
#endif
};
