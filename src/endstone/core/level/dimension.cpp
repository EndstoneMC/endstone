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

#include "endstone/core/level/dimension.h"

#include <ranges>

#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "endstone/core/actor/item.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/level/chunk.h"
#include "endstone/core/level/level.h"
#include "endstone/player.h"

namespace endstone::core {

namespace {
std::uint64_t chunkKey(int x, int z)
{
    return static_cast<std::uint64_t>(static_cast<std::uint32_t>(x)) << 32 | static_cast<std::uint32_t>(z);
}
}  // namespace

EndstoneDimension::EndstoneDimension(WeakRef<::Dimension> dimension, EndstoneLevel &level)
    : dimension_(std::move(dimension)), level_(level)
{
}

DimensionId EndstoneDimension::getId() const
{
    return DimensionId(getHandle().getTypeId());
}

std::string EndstoneDimension::getTranslationKey() const
{
    return getHandle().getLocalizationKey();
}

Level &EndstoneDimension::getLevel() const
{
    return level_;
}

bool EndstoneDimension::isValid() const
{
    return dimension_.isSet();
}

std::unique_ptr<Block> EndstoneDimension::getBlockAt(int x, int y, int z) const
{
    return EndstoneBlock::at(getHandle().getBlockSourceFromMainChunkSource(), BlockPos(x, y, z));
}

std::unique_ptr<Block> EndstoneDimension::getBlockAt(Location location) const
{
    return getBlockAt(location.getBlockX(), location.getBlockY(), location.getBlockZ());
}

int EndstoneDimension::getHighestBlockYAt(int x, int z) const
{
    const auto height = getHandle().getBlockSourceFromMainChunkSource().getHeight(
        [](auto &block) { return block.getName() != BedrockBlockNames::Air; }, x, z);
    return height - 1;
}

std::unique_ptr<Block> EndstoneDimension::getHighestBlockAt(int x, int z) const
{
    return getBlockAt(x, getHighestBlockYAt(x, z), z);
}

std::unique_ptr<Block> EndstoneDimension::getHighestBlockAt(Location location) const
{
    return getHighestBlockAt(location.getBlockX(), location.getBlockZ());
}

std::vector<std::unique_ptr<Chunk>> EndstoneDimension::getLoadedChunks()
{
    std::vector<std::unique_ptr<Chunk>> chunks;
    for (const auto &weak_lc : getHandle().getChunkSource().getStorage() | std::views::values) {
        if (weak_lc.expired()) {
            continue;
        }
        if (auto chunk = weak_lc.lock(); chunk && chunk->getState() >= ChunkState::Loaded) {
            chunks.emplace_back(std::make_unique<EndstoneChunk>(*chunk));
        }
    }
    return chunks;
}

bool EndstoneDimension::isChunkLoaded(int x, int z) const
{
    const auto chunk = getHandle().getChunkSource().getExistingChunk(ChunkPos(x, z));
    return chunk && chunk->getState() >= ChunkState::Loaded;
}

bool EndstoneDimension::loadChunk(int x, int z)
{
    auto chunk = getHandle().getChunkSource().getOrLoadChunk(ChunkPos(x, z), ::ChunkSource::LoadMode::Deferred, false);
    if (!chunk) {
        return false;
    }
    loaded_chunks_[chunkKey(x, z)] = std::move(chunk);
    return true;
}

bool EndstoneDimension::unloadChunk(int x, int z)
{
    loaded_chunks_.erase(chunkKey(x, z));
    getHandle().flushLevelChunkGarbageCollector();
    return true;
}

NotNull<Item> EndstoneDimension::dropItem(const Location location, const ItemStack &item)
{
    auto item_stack = EndstoneItemStack::toMinecraft(item);
    auto *actor = getHandle().getLevel().getSpawner().spawnItem(
        getHandle().getBlockSourceFromMainChunkSource(), item_stack, nullptr,
        Vec3{location.getX(), location.getY(), location.getZ()}, 10);
    Preconditions::checkState(actor != nullptr, "Unable to drop item at the specified location.");
    return actor->getEndstoneActor<EndstoneItem>();
}

Nullable<Actor> EndstoneDimension::spawnActor(Location location, ActorTypeId type)
{
    auto &actor_factory = level_.getHandle().getActorFactory();
    const auto id = ActorDefinitionIdentifier(std::string(type));
    auto entity = actor_factory.createSpawnedActor(id, nullptr, {location.getX(), location.getY(), location.getZ()},
                                                   {location.getPitch(), location.getYaw()});
    const auto *actor =
        level_.getHandle().addEntity(getHandle().getBlockSourceFromMainChunkSource(), std::move(entity));
    if (!actor) {
        return nullptr;
    }
    return actor->getEndstoneActor();
}

std::vector<NotNull<Actor>> EndstoneDimension::getActors() const
{
    std::vector<NotNull<Actor>> result;
    for (const auto &actor : level_.getActors()) {
        if (actor->getDimension() != getSelf()) {
            continue;
        }
        result.push_back(actor);
    }
    return result;
}

std::vector<NotNull<Mob>> EndstoneDimension::getMobs() const
{
    std::vector<NotNull<Mob>> result;
    for (const auto &actor : getActors()) {
        if (actor->is<Mob>()) {
            result.push_back(actor.cast<Mob>());
        }
    }
    return result;
}

std::vector<NotNull<Player>> EndstoneDimension::getPlayers() const
{
    std::vector<NotNull<Player>> result;
    for (const auto &actor : getActors()) {
        if (actor->is<Player>()) {
            result.push_back(actor.cast<Player>());
        }
    }
    return result;
}

::Dimension &EndstoneDimension::getHandle() const
{
    auto handle = dimension_.unwrap();
    if (!handle) {
        throw std::runtime_error("Trying to access a dimension that is no longer valid.");
    }
    return *handle;
}

NotNull<EndstoneDimension> EndstoneDimension::getSelf() const
{
    return const_cast<EndstoneDimension *>(this)->shared_from_this();
}
}  // namespace endstone::core

endstone::NotNull<endstone::Dimension> Dimension::getEndstoneDimension() const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    return server.getEndstoneLevel()->getDimension(getDimensionId());
}
