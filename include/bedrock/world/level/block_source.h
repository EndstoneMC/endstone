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

#include "bedrock/world/actor/actor_types.h"
#include "bedrock/world/item/item_stack_base.h"
#include "bedrock/world/level/block_source_interface.h"
#include "bedrock/world/level/chunk/level_chunk.h"

class BlockSource : public IBlockSource, public std::enable_shared_from_this<BlockSource> {
public:
    ~BlockSource() override = 0;
    void fetchAABBs(std::vector<AABB> &, AABB const &, bool) const override = 0;
    void fetchCollisionShapes(std::vector<AABB> &, AABB const &, bool, optional_ref<GetCollisionShapeInterface const>,
                              std::vector<AABB> *) const override = 0;

    virtual WeakRef<BlockSource> getWeakRef() = 0;
    virtual void addListener(BlockSourceListener &) = 0;
    virtual void removeListener(BlockSourceListener &) = 0;
    virtual gsl::span<gsl::not_null<Actor *>> fetchEntities(Actor const *, AABB const &, bool, bool) = 0;
    virtual gsl::span<gsl::not_null<Actor *>> fetchEntities(ActorType, AABB const &, Actor const *,
                                                            std::function<bool(Actor *)>) = 0;
    virtual bool setBlock(BlockPos const &, Block const &, int, ActorBlockSyncMessage const *, Actor *) = 0;
    [[nodiscard]] virtual std::int16_t getMinHeight() const = 0;
    [[nodiscard]] virtual std::int16_t getMaxHeight() const = 0;
    [[nodiscard]] virtual Dimension &getDimension() const = 0;
    virtual Dimension &getDimension() = 0;
    [[nodiscard]] virtual Dimension const &getDimensionConst() const = 0;
    [[nodiscard]] virtual LevelChunk *getChunkAt(BlockPos const &) const = 0;
    [[nodiscard]] virtual LevelChunk *getChunk(int, int) const = 0;
    [[nodiscard]] virtual LevelChunk *getChunk(ChunkPos const &) const = 0;
    virtual Level &getLevel() = 0;
    [[nodiscard]] virtual ILevel &getILevel() const = 0;
    virtual std::vector<AABB> &fetchAABBs(AABB const &, bool) = 0;
    virtual std::vector<AABB> &fetchCollisionShapes(AABB const &, bool, std::optional<EntityContext const>,
                                                    std::vector<AABB> *) = 0;
    virtual HitResult clip(Vec3 const &, Vec3 const &, bool, ShapeType, int, bool, bool, Actor *,
                           std::function<bool(BlockSource const &, Block const &, bool)> const &) const = 0;
    virtual ChunkSource &getChunkSource() = 0;
    [[nodiscard]] virtual bool isSolidBlockingBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual bool isSolidBlockingBlock(int, int, int) const = 0;
    [[nodiscard]] virtual bool areChunksFullyLoaded(BlockPos const &, int) const = 0;
    [[nodiscard]] virtual bool canDoBlockDrops() const = 0;
    [[nodiscard]] virtual bool canDoContainedItemDrops() const = 0;
    [[nodiscard]] virtual bool isInstaticking(BlockPos const &) const = 0;
    virtual void updateCheckForValidityState(bool) = 0;
    virtual bool checkBlockPermissions(Actor &, BlockPos const &, FacingID, ItemStackBase const &, bool) = 0;
};
