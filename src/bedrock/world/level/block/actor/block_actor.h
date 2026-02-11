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

#include <string>

#include "bedrock/core/container/enum_set.h"
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
    virtual void load(ILevel &, const CompoundTag &, DataLoadHelper &) = 0;
    virtual bool save(CompoundTag &, const SaveContext &) const = 0;
    virtual bool saveItemInstanceData(CompoundTag &, const SaveContext &) const = 0;
    virtual void saveBlockData(CompoundTag &, BlockSource &) const = 0;
    virtual void loadBlockData(const CompoundTag &, BlockSource &, DataLoadHelper &) = 0;
    virtual void onCustomTagLoadDone(BlockSource &) = 0;
    [[nodiscard]] virtual bool isPermanentlyRendered() const = 0;
    [[nodiscard]] virtual bool isWithinRenderDistance(const Vec3 &) const = 0;
    virtual void tick(BlockSource &) = 0;

    void setChanged() { properties_.insert(Property::Changed); }

    [[nodiscard]] bool isChanged() const { return properties_.contains(Property::Changed); }

    [[nodiscard]] BlockActorType getType() const { return type_; }

    int tick_count;

protected:
    int repair_cost_;
    BlockPos position_;
    AABB bb_;
    BlockActorRendererId renderer_id_;
    const BlockActorType type_;
    Properties properties_;
    Bedrock::Safety::RedactableString custom_name_;
    std::string filtered_custom_name_;
    ActorTerrainInterlockData terrain_interlock_data_;

    // private:
    //     bool changed_;
};
BEDROCK_STATIC_ASSERT_SIZE(BlockActor, 184, 160);
