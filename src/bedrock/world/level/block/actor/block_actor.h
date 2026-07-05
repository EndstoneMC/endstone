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

#include <memory>
#include <string>

#include "bedrock/core/container/enum_set.h"
#include "bedrock/forward.h"
#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/actor/actor_terrain_interlock_data.h"
#include "bedrock/world/item/save_context.h"
#include "bedrock/world/level/block/actor/block_actor_type.h"
#include "bedrock/world/level/block/block.h"

class ILevel;

class BlockActor {
public:
    enum class Property : uint8_t {
        Changed = 0,
        Movable = 1,
        ClientSideOnly = 2,
        SaveCustomName = 3,
        CanRenderCustomName = 4,
        _count = 5,
    };
    using Properties = Bedrock::EnumSet<Property, Property::_count>;

    BlockActor(BlockActorType, const BlockPos &, const std::string &);
    virtual ~BlockActor() = default;

    [[nodiscard]] BlockActorType getType() const { return type_; }

protected:
    BlockPos position_;          // +8
    const BlockActorType type_;  // +20
    std::unique_ptr<BlockActorDynamicPropertiesComponent> dynamic_properties_;  // +24
};
BEDROCK_STATIC_ASSERT_SIZE(BlockActor, 32, 32);
