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

#include "bedrock/forward.h"
#include "bedrock/shared_types/height.h"
#include "bedrock/world/actor/actor_types.h"
#include "bedrock/world/events/gameevents/game_event.h"
#include "bedrock/world/item/item_stack_base.h"
#include "bedrock/world/level/biome/biome.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_type.h"
#include "bedrock/world/level/block_source_listener.h"
#include "bedrock/world/level/chunk/level_chunk.h"
#include "bedrock/world/level/clip_parameters.h"
#include "bedrock/world/level/dimension/dimension_type.h"
#include "bedrock/world/level/level_seed.h"

class ILevel;
class Level;
class Material;

using ActorSpan = gsl::span<gsl::not_null<Actor *>>;
using ConstActorSpan = gsl::span<gsl::not_null<const Actor *>>;
using GetBlockFunction = std::function<const Block &(const BlockPos &)>;

class IConstBlockSource {
public:
    virtual ~IConstBlockSource() = 0;
    [[nodiscard]] virtual Block const &getBlock(int, int, int) const = 0;
    [[nodiscard]] virtual Block const &getBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getBlock(BlockPos const &, std::uint32_t) const = 0;
    [[nodiscard]] virtual BlockActor const *getBlockEntity(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getExtraBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getLiquidBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual bool hasBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual bool containsAnyLiquid(AABB const &) const = 0;
    [[nodiscard]] virtual bool containsMaterial(AABB const &, MaterialType) const = 0;
    [[nodiscard]] virtual bool isInWall(const Vec3 &) const = 0;
    [[nodiscard]] virtual bool isUnderWater(Vec3 const &, Block const &) const = 0;
    [[nodiscard]] virtual Material const &getMaterial(BlockPos const &) const = 0;
    [[nodiscard]] virtual Material const &getMaterial(int, int, int) const = 0;
    [[nodiscard]] virtual bool hasBorderBlock(BlockPos) const = 0;
    [[nodiscard]] virtual bool hasChunksAt(Bounds const &, bool) const = 0;
    [[nodiscard]] virtual bool hasChunksAt(BlockPos const &, int, bool) const = 0;
    [[nodiscard]] virtual bool hasChunksAt(AABB const &, bool) const = 0;
    [[nodiscard]] virtual DimensionType getDimensionId() const = 0;
    virtual void fetchAABBs(std::vector<AABB> &, AABB const &, bool) const = 0;
    virtual void fetchCollisionShapes(std::vector<AABB> &, AABB const &, bool,
                                      optional_ref<GetCollisionShapeInterface const>, std::vector<AABB> *) const = 0;
    virtual void fetchCollisionShapesAndBlocks(std::vector<BlockSourceVisitor::CollisionShape> &, AABB const &, bool,
                                               optional_ref<GetCollisionShapeInterface const>,
                                               std::vector<AABB> *) const = 0;
    virtual AABB getTallestCollisionShape(AABB const &, float *, bool,
                                          optional_ref<GetCollisionShapeInterface const>) const = 0;
    [[nodiscard]] virtual float getBrightness(BlockPos const &) const = 0;
    [[nodiscard]] virtual float getVisualLiquidHeight(const Vec3 &) const = 0;
};

class IBlockSource : public IConstBlockSource {
public:
    using Listener = BlockSourceListener;
    using ListenerVector = std::vector<BlockSourceListener *>;

    ~IBlockSource() override = 0;
    void fetchAABBs(std::vector<AABB> &, AABB const &, bool) const override = 0;
    void fetchCollisionShapes(std::vector<AABB> &, AABB const &, bool, optional_ref<GetCollisionShapeInterface const>,
                              std::vector<AABB> *) const override = 0;
    virtual WeakRef<BlockSource> getWeakRef() = 0;
    virtual void addListener(Listener &) = 0;
    virtual void removeListener(Listener &) = 0;
    virtual ActorSpan fetchEntities(Actor const *, AABB const &, bool, bool) = 0;
    virtual ActorSpan fetchEntities(ActorType, AABB const &, Actor const *, std::function<bool(Actor *)>) = 0;
    virtual bool setBlock(BlockPos const &, Block const &, int, ActorBlockSyncMessage const *, Actor *) = 0;
    [[nodiscard]] virtual Height getMinHeight() const = 0;
    [[nodiscard]] virtual Height getMaxHeight() const = 0;
    [[nodiscard]] virtual Dimension &getDimension() const = 0;
    [[nodiscard]] virtual const Dimension &getDimensionConst() const = 0;
    virtual Dimension &getDimension() = 0;
    [[nodiscard]] virtual LevelChunk *getChunkAt(BlockPos const &) const = 0;
    [[nodiscard]] virtual LevelChunk *getChunk(int, int) const = 0;
    [[nodiscard]] virtual LevelChunk *getChunk(ChunkPos const &) const = 0;
    virtual Level &getLevel() = 0;
    [[nodiscard]] virtual ILevel &getILevel() const = 0;
    [[nodiscard]] virtual LevelSeed64 getLevelSeed64() const = 0;
    [[nodiscard]] virtual Height getAboveTopSolidBlock(int, int, bool, bool) const = 0;
    [[nodiscard]] virtual Height getAboveTopSolidBlock(const BlockPos &, bool, bool, bool) const = 0;
    [[nodiscard]] virtual Height getHeight(std::function<bool(const Block &)> const &, BlockPos const &) const = 0;
    [[nodiscard]] virtual Height getHeight(std::function<bool(const Block &)> const &, int, int) const = 0;
    virtual std::vector<AABB> &fetchAABBs(AABB const &, bool) = 0;
    virtual std::vector<AABB> &fetchCollisionShapes(AABB const &, bool, std::optional<EntityContext const>,
                                                    std::vector<AABB> *) = 0;
    virtual HitResult clip(Vec3 const &, Vec3 const &, bool, ShapeType, int, bool, bool, Actor *,
                           std::function<bool(BlockSource const &, Block const &, bool)> const &, bool) const = 0;
    [[nodiscard]] virtual HitResult clip(const ClipParameters &) const = 0;
    virtual ChunkSource &getChunkSource() = 0;
    [[nodiscard]] virtual bool isSolidBlockingBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual bool isSolidBlockingBlock(int, int, int) const = 0;
    [[nodiscard]] virtual bool areChunksFullyLoaded(BlockPos const &, int) const = 0;
    virtual bool mayPlace(const Block &, const BlockPos &, FacingID, Actor *, bool, Vec3) = 0;
    [[nodiscard]] virtual bool canDoBlockDrops() const = 0;
    [[nodiscard]] virtual bool canDoContainedItemDrops() const = 0;
    [[nodiscard]] virtual bool isInstaticking(BlockPos const &) const = 0;
    virtual void updateCheckForValidityState(bool) = 0;
    virtual bool checkBlockPermissions(Actor &, BlockPos const &, FacingID, ItemStackBase const &, bool) = 0;
    virtual bool removeBlock(BlockPos const &) = 0;
    virtual void postGameEvent(Actor *, const GameEvent &, const BlockPos &, const Block *) = 0;
};

class BlockSource : public IBlockSource,
                    public EnableGetWeakRef<BlockSource>,
                    public std::enable_shared_from_this<BlockSource> {
public:
    explicit BlockSource(ChunkSource &, bool, bool);
    BlockSource(Level &, Dimension &, ChunkSource &, bool, bool, bool);
    BlockSource(ILevel &, ChunkSource &, bool, bool);

    [[nodiscard]] bool isEmptyBlock(const BlockPos &pos) const;
    [[nodiscard]] const Biome &getBiome(const BlockPos &) const;
};
