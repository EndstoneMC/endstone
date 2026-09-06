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

#include "endstone/core/level/chunk.h"

#include <random>
#include <ranges>
#include <stdexcept>

#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/core/block/block.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/server.h"
#include "endstone/level/dimension.h"

namespace endstone::core {

EndstoneChunk::EndstoneChunk(WeakRef<::Dimension> dimension, int x, int z)
    : dimension_(std::move(dimension)), x_(x), z_(z)
{
}

EndstoneChunk::EndstoneChunk(const LevelChunk &chunk)
    : EndstoneChunk(chunk.getDimension().getWeakRef(), chunk.getPosition().x, chunk.getPosition().z)
{
}

int EndstoneChunk::getX() const
{
    return x_;
}

int EndstoneChunk::getZ() const
{
    return z_;
}

Level &EndstoneChunk::getLevel() const
{
    return getDimension()->getLevel();
}

NotNull<Dimension> EndstoneChunk::getDimension() const
{
    const auto handle = dimension_.unwrap();
    if (!handle) {
        throw std::runtime_error("Trying to access a dimension that is no longer valid.");
    }
    return handle->getEndstoneDimension();
}

NotNull<Block> EndstoneChunk::getBlock(int x, int y, int z) const
{
    const auto dimension = getDimension().cast<EndstoneDimension>();
    auto &block_source = dimension->getHandle().getBlockSourceFromMainChunkSource();
    Preconditions::checkArgument(x >= 0 && x < 16, "x must be between 0 and 15, got {}.", x);
    Preconditions::checkArgument(y >= block_source.getMinHeight() && y < block_source.getMaxHeight(),
                                 "y must be between {} (inclusive) and {} (exclusive), got {}.",
                                 block_source.getMinHeight(), block_source.getMaxHeight(), y);
    Preconditions::checkArgument(z >= 0 && z < 16, "z must be between 0 and 15, got {}.", z);
    const BlockPos block_pos{(x_ << 4) + x, y, (z_ << 4) + z};
    return EndstoneBlock::at(block_source, block_pos);
}

std::vector<NotNull<Actor>> EndstoneChunk::getEntities() const
{
    if (!isLoaded()) {
        return {};
    }
    std::vector<NotNull<Actor>> result;
    for (const auto &actor : getDimension()->getActors()) {
        const auto location = actor->getLocation();
        if ((location.getBlockX() >> 4) == x_ && (location.getBlockZ() >> 4) == z_) {
            result.push_back(actor);
        }
    }
    return result;
}

bool EndstoneChunk::isGenerated() const
{
    return getDimension()->isChunkGenerated(x_, z_);
}

bool EndstoneChunk::isSlimeChunk() const
{
    const auto seed = (static_cast<std::uint32_t>(x_) * 0x1f1f1f1fU) ^ static_cast<std::uint32_t>(z_);
    std::mt19937 random(seed);
    return random() % 10 == 0;
}

bool EndstoneChunk::isLoaded() const
{
    return getDimension()->isChunkLoaded(x_, z_);
}

bool EndstoneChunk::load()
{
    return getDimension()->loadChunk(x_, z_);
}

bool EndstoneChunk::load(bool generate)
{
    return getDimension()->loadChunk(x_, z_, generate);
}

bool EndstoneChunk::unload()
{
    return getDimension()->unloadChunk(x_, z_);
}

bool EndstoneChunk::isForceLoaded() const
{
    return getDimension()->isChunkForceLoaded(x_, z_);
}

void EndstoneChunk::setForceLoaded(bool forced)
{
    getDimension()->setChunkForceLoaded(x_, z_, forced);
}

bool EndstoneChunk::addPluginChunkTicket(Plugin &plugin)
{
    return getDimension()->addPluginChunkTicket(x_, z_, plugin);
}

bool EndstoneChunk::removePluginChunkTicket(Plugin &plugin)
{
    return getDimension()->removePluginChunkTicket(x_, z_, plugin);
}

std::vector<NotNull<BlockState>> EndstoneChunk::getBlockActors() const
{
    const auto dimension = getDimension().cast<EndstoneDimension>();
    const auto chunk = dimension->getHandle().getChunkSource().getExistingChunk(ChunkPos(x_, z_));
    if (!chunk || chunk->getState() < ChunkState::Loaded) {
        return {};
    }
    auto &block_source = dimension->getHandle().getBlockSourceFromMainChunkSource();
    const auto min_height = block_source.getMinHeight();
    std::vector<NotNull<BlockState>> block_actors;
    for (const auto &pos : chunk->getBlockEntities() | std::views::keys) {
        const BlockPos block_pos{(x_ << 4) + pos.x, min_height + pos.y.getVal(), (z_ << 4) + pos.z};
        block_actors.push_back(EndstoneBlock::at(block_source, block_pos)->captureState(true));
    }
    return block_actors;
}

std::vector<Plugin *> EndstoneChunk::getPluginChunkTickets() const
{
    return getDimension()->getPluginChunkTickets(x_, z_);
}

}  // namespace endstone::core
