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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/chunk/chunk_source.h"
#include "bedrock/world/level/saveddata/maps/map_decoration.h"

class MapItemTrackedActor {
    enum class Type : int {
        Entity = 0,
        BlockEntity = 1,
        Other = 2,
    };

public:
    struct UniqueId {
        Type type;
        ActorUniqueID key_entity_id;
        BlockPos key_block_pos;
        UniqueId() = default;
        UniqueId(const UniqueId &) = default;
        UniqueId(const ActorUniqueID &entity_id) : type(Type::Entity), key_entity_id(entity_id) {}
        UniqueId(const BlockPos &block_pos) : type(Type::BlockEntity), key_block_pos(block_pos) {}
        UniqueId &operator=(const UniqueId &) = default;
        bool operator==(const UniqueId &other) const
        {
            if (type == Type::BlockEntity) {
                return key_block_pos == other.key_block_pos;
            }
            return key_entity_id == other.key_entity_id;
        }
    };
    static_assert(sizeof(UniqueId) == 32);

    MapItemTrackedActor(const UniqueId &, BlockSource &);
    void setPixelDirty(std::uint32_t, std::uint32_t);
    Actor *getEntity(BlockSource &);
    BlockActor *getBlockEntity(BlockSource &);
    const UniqueId &getUniqueId() const;
    float getDecorationRotation(BlockSource &);
    MapDecoration::Type getDecorationType() const;
    void setDecorationType(MapDecoration::Type);
    ChunkViewSource &getChunkViewSource();
    DimensionType getDimensionId();
    void setNeedsResend();

private:
    UniqueId unique_id_;
    bool needs_resend_;
    std::uint32_t min_dirty_x_;
    std::uint32_t min_dirty_y_;
    std::uint32_t max_dirty_x_;
    std::uint32_t max_dirty_y_;
    int tick_;
    float last_rotation_;
    MapDecoration::Type decoration_type_;
    DimensionType dimension_id_;
    std::shared_ptr<ChunkViewSource> chunk_view_source_;
};
static_assert(sizeof(MapItemTrackedActor) == 88);
