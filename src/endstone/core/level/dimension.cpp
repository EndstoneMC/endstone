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

#include <endstone/core/actor/item.h>
#include <endstone/core/inventory/item_stack.h>

#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "endstone/core/block/block.h"
#include "endstone/core/level/chunk.h"
#include "endstone/core/level/level.h"

namespace endstone::core {
EndstoneDimension::EndstoneDimension(WeakRef<::Dimension> dimension, EndstoneLevel &level)
    : dimension_(std::move(dimension)), level_(level)
{
}

std::string EndstoneDimension::getName() const
{
    return getHandle().getName();
}

Dimension::Type EndstoneDimension::getType() const
{
    switch (getHandle().getDimensionId().runtime_id) {
    case VanillaDimensions::Overworld.runtime_id:
        return Type::Overworld;
    case VanillaDimensions::Nether.runtime_id:
        return Type::Nether;
    case VanillaDimensions::TheEnd.runtime_id:
        return Type::TheEnd;
    default:
        return Type::Custom;
    }
}

Level &EndstoneDimension::getLevel() const
{
    return level_;
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

Item &EndstoneDimension::dropItem(const Location location, const ItemStack &item)
{
    auto item_stack = EndstoneItemStack::toMinecraft(item);
    auto *actor = getHandle().getLevel().getSpawner().spawnItem(
        getHandle().getBlockSourceFromMainChunkSource(), item_stack, nullptr,
        Vec3{location.getX(), location.getY(), location.getZ()}, 10);
    return actor->getEndstoneActor<EndstoneItem>();
}

Actor *EndstoneDimension::spawnActor(Location location, std::string type)
{
    auto &actor_factory = level_.getHandle().getActorFactory();
    const auto id = ActorDefinitionIdentifier(type);
    auto entity = actor_factory.createSpawnedActor(id, nullptr, {location.getX(), location.getY(), location.getZ()},
                                                   {location.getPitch(), location.getYaw()});
    const auto *actor =
        level_.getHandle().addEntity(getHandle().getBlockSourceFromMainChunkSource(), std::move(entity));
    if (!actor) {
        return nullptr;
    }
    return &actor->getEndstoneActor();
}

std::vector<Actor *> EndstoneDimension::getActors() const
{
    std::vector<Actor *> result;
    for (const auto &actor : level_.getActors()) {
        if (!actor) {
            continue;
        }
        if (&actor->getDimension() != this) {
            continue;
        }
        result.push_back(actor);
    }
    return result;
}

::Dimension &EndstoneDimension::getHandle() const
{
    if (!dimension_.isSet()) {
        throw std::runtime_error("Trying to access a dimension that is no longer valid.");
    }
    return *dimension_.unwrap();
}
}  // namespace endstone::core

endstone::Dimension &Dimension::getEndstoneDimension() const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    return *server.getEndstoneLevel()->getDimension(getDimensionId().runtime_id);
}
