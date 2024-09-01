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

#include "bedrock/forward.h"

#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block/block_legacy.h"
#include "bedrock/world/level/dimension/dimension_interface.h"
#include "bedrock/world/level/material/material.h"
#include "bedrock/world/level/material/material_type.h"

class IConstBlockSource {
public:
    virtual ~IConstBlockSource() = 0;
};

class BlockSource;
class ILevel;
class Level;

class IBlockSource : public IConstBlockSource {
public:
    ~IBlockSource() override = 0;
    [[nodiscard]] virtual Block const &getBlock(int, int, int) const = 0;
    [[nodiscard]] virtual Block const &getBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getBlock(BlockPos const &, std::uint32_t) const = 0;
    [[nodiscard]] virtual BlockActor const *getBlockEntity(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getExtraBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual Block const &getLiquidBlock(BlockPos const &) const = 0;
    [[nodiscard]] virtual bool hasBlock(BlockPos const &) const = 0;
    virtual bool removeBlock(BlockPos const &) = 0;
    [[nodiscard]] virtual bool containsAnyLiquid(AABB const &) const = 0;
    [[nodiscard]] virtual bool containsMaterial(AABB const &, MaterialType) const = 0;
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
    virtual void postGameEvent(Actor *, GameEvent const &, BlockPos const &, Block const *) = 0;
};
