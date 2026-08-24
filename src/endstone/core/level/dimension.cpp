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

#include <algorithm>
#include <ranges>

#include "bedrock/entity/components/actor_owner_component.h"
#include "bedrock/world/level/block/bedrock_block_names.h"
#include "bedrock/world/level/ticking/ticking_areas_manager.h"
#include "endstone/core/actor/item.h"
#include "endstone/core/block/block.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/level/chunk.h"
#include "endstone/core/level/level.h"
#include "endstone/player.h"
#include "endstone/plugin/plugin.h"

namespace endstone::core {

namespace {
// Any set scope marks the area as scoped, which is what keeps it out of /tickingarea list and off disk.
constexpr std::uint64_t endstone_area_scope = 1;
constexpr std::uint64_t endstone_area_id_prefix = 0x656E6473746F6E65ULL;  // "endstone"

std::uint64_t chunkKey(int x, int z)
{
    return static_cast<std::uint64_t>(static_cast<std::uint32_t>(x)) << 32 | static_cast<std::uint32_t>(z);
}

mce::UUID nextAreaId()
{
    static std::uint64_t counter = 0;
    return {endstone_area_id_prefix, ++counter};
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

NotNull<Block> EndstoneDimension::getBlockAt(int x, int y, int z) const
{
    return EndstoneBlock::at(getHandle().getBlockSourceFromMainChunkSource(), BlockPos(x, y, z));
}

NotNull<Block> EndstoneDimension::getBlockAt(Location location) const
{
    return getBlockAt(location.getBlockX(), location.getBlockY(), location.getBlockZ());
}

int EndstoneDimension::getHighestBlockYAt(int x, int z) const
{
    const auto height = getHandle().getBlockSourceFromMainChunkSource().getHeight(
        [](auto &block) { return block.getName() != BedrockBlockNames::Air; }, x, z);
    return height - 1;
}

NotNull<Block> EndstoneDimension::getHighestBlockAt(int x, int z) const
{
    return getBlockAt(x, getHighestBlockYAt(x, z), z);
}

NotNull<Block> EndstoneDimension::getHighestBlockAt(Location location) const
{
    return getHighestBlockAt(location.getBlockX(), location.getBlockZ());
}

std::vector<NotNull<Chunk>> EndstoneDimension::getLoadedChunks()
{
    std::vector<NotNull<Chunk>> chunks;
    for (const auto &weak_lc : getHandle().getChunkSource().getStorage() | std::views::values) {
        if (weak_lc.expired()) {
            continue;
        }
        if (auto chunk = weak_lc.lock(); chunk && chunk->getState() >= ChunkState::Loaded) {
            chunks.emplace_back(std::make_shared<EndstoneChunk>(*chunk));
        }
    }
    return chunks;
}

bool EndstoneDimension::isChunkLoaded(int x, int z) const
{
    const auto chunk = getHandle().getChunkSource().getExistingChunk(ChunkPos(x, z));
    return chunk && chunk->getState() >= ChunkState::Loaded;
}

bool EndstoneDimension::isChunkGenerated(int x, int z) const
{
    return isChunkLoaded(x, z) || getHandle().getChunkSource().isChunkSaved(ChunkPos(x, z));
}

void EndstoneDimension::checkServerThread() const
{
    Preconditions::checkState(level_.getServer().isPrimaryThread(),
                              "Chunks can only be loaded or unloaded from the server thread.");
}

mce::UUID EndstoneDimension::pinChunk(int x, int z, bool generate)
{
    checkServerThread();
    if (!generate && !isChunkGenerated(x, z)) {
        return mce::UUID::EMPTY;
    }
    const auto context = level_.getHandle().getTickingAreasMgr().addTransientScopedArea(
        getHandle().getDimensionId(), {}, endstone_area_scope, nextAreaId(), Bounds::forChunks(x, z, x, z));
    return context.uuid;
}

void EndstoneDimension::releaseTicket(std::unordered_map<std::uint64_t, ChunkTicket>::iterator it)
{
    if (it != chunk_tickets_.end() && it->second.plugins.empty() && !it->second.server_owned) {
        level_.getHandle().getTickingAreasMgr().removeScopedAreas({it->second.area_id});
        chunk_tickets_.erase(it);
    }
}

bool EndstoneDimension::loadChunk(int x, int z)
{
    return loadChunk(x, z, true);
}

bool EndstoneDimension::loadChunk(int x, int z, bool generate)
{
    const auto key = chunkKey(x, z);
    auto it = chunk_tickets_.find(key);
    if (it == chunk_tickets_.end()) {
        const auto area_id = pinChunk(x, z, generate);
        if (area_id == mce::UUID::EMPTY) {
            return false;
        }
        it = chunk_tickets_.emplace(key, ChunkTicket{.area_id = area_id}).first;
    }
    it->second.server_owned = true;
    return true;
}

bool EndstoneDimension::unloadChunk(int x, int z)
{
    checkServerThread();
    const auto was_loaded = isChunkLoaded(x, z);
    unloadChunkRequest(x, z);
    if (!was_loaded) {
        return true;
    }
    getHandle().flushLevelChunkGarbageCollector();
    return !isChunkLoaded(x, z);
}

bool EndstoneDimension::unloadChunkRequest(int x, int z)
{
    checkServerThread();
    Preconditions::checkState(isValid(), "Trying to access a dimension that is no longer valid.");
    const auto it = chunk_tickets_.find(chunkKey(x, z));
    if (it != chunk_tickets_.end()) {
        it->second.server_owned = false;
        releaseTicket(it);
    }
    return true;
}

bool EndstoneDimension::addPluginChunkTicket(int x, int z, Plugin &plugin)
{
    checkServerThread();
    Preconditions::checkArgument(plugin.isEnabled(), "Plugin is not enabled.");
    const auto key = chunkKey(x, z);
    auto it = chunk_tickets_.find(key);
    if (it != chunk_tickets_.end() &&
        std::ranges::find(it->second.plugins, &plugin) != it->second.plugins.end()) {
        return false;
    }
    if (it == chunk_tickets_.end()) {
        const auto area_id = pinChunk(x, z, true);
        if (area_id == mce::UUID::EMPTY) {
            return false;
        }
        it = chunk_tickets_.emplace(key, ChunkTicket{.area_id = area_id}).first;
    }
    it->second.plugins.push_back(&plugin);
    return true;
}

bool EndstoneDimension::removePluginChunkTicket(int x, int z, Plugin &plugin)
{
    checkServerThread();
    const auto it = chunk_tickets_.find(chunkKey(x, z));
    if (it == chunk_tickets_.end()) {
        return false;
    }
    if (std::erase(it->second.plugins, &plugin) == 0) {
        return false;
    }
    releaseTicket(it);
    return true;
}

void EndstoneDimension::removePluginChunkTickets(Plugin &plugin)
{
    std::vector<mce::UUID> released;
    for (auto it = chunk_tickets_.begin(); it != chunk_tickets_.end();) {
        std::erase(it->second.plugins, &plugin);
        if (it->second.plugins.empty() && !it->second.server_owned) {
            released.push_back(it->second.area_id);
            it = chunk_tickets_.erase(it);
        }
        else {
            ++it;
        }
    }
    if (!released.empty()) {
        level_.getHandle().getTickingAreasMgr().removeScopedAreas(released);
    }
}

std::vector<Plugin *> EndstoneDimension::getPluginChunkTickets(int x, int z) const
{
    const auto it = chunk_tickets_.find(chunkKey(x, z));
    if (it == chunk_tickets_.end()) {
        return {};
    }
    return it->second.plugins;
}

std::unordered_map<Plugin *, std::vector<NotNull<Chunk>>> EndstoneDimension::getPluginChunkTickets() const
{
    std::unordered_map<Plugin *, std::vector<NotNull<Chunk>>> result;
    for (const auto &[key, ticket] : chunk_tickets_) {
        if (ticket.plugins.empty()) {
            continue;
        }
        const auto x = static_cast<int>(static_cast<std::uint32_t>(key >> 32));
        const auto z = static_cast<int>(static_cast<std::uint32_t>(key));
        for (auto *plugin : ticket.plugins) {
            result[plugin].emplace_back(std::make_shared<EndstoneChunk>(dimension_, x, z));
        }
    }
    return result;
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
        if (actor->getDimension() != self()) {
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

NotNull<EndstoneDimension> EndstoneDimension::self() const
{
    return const_cast<EndstoneDimension *>(this)->shared_from_this();
}
}  // namespace endstone::core

endstone::NotNull<endstone::Dimension> Dimension::getEndstoneDimension() const
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    return server.getEndstoneLevel()->getDimension(getDimensionId());
}
