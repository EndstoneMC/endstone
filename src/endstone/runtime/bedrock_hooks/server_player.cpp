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

#include "bedrock/server/server_player.h"

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "bedrock/server/network_chunk_publisher.h"
#include "bedrock/world/level/chunk/chunk_view_source.h"
#include "bedrock/world/level/chunk/level_chunk.h"
#include "bedrock/world/level/chunk/level_chunk_event_manager.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/level/location.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_portal_event.h"
#include "endstone/runtime/hook.h"

namespace {
using ChunkQueue = std::unordered_map<ChunkPositionAndDimension, std::weak_ptr<LevelChunk>>;

enum class QueuedChunkState {
    Sendable,
    OutOfView,
    NotLoaded,
};

struct ParkedChunk {
    ChunkQueue::node_type node;
    QueuedChunkState state;
};

using ParkedChunks = std::unordered_map<ChunkPositionAndDimension, ParkedChunk>;

struct ParkedChunksComponent {
    ParkedChunksComponent() = default;
    ParkedChunksComponent(const ParkedChunksComponent &) = delete;
    ParkedChunksComponent(ParkedChunksComponent &&) = default;
    ParkedChunksComponent &operator=(const ParkedChunksComponent &) = delete;
    ParkedChunksComponent &operator=(ParkedChunksComponent &&) = default;

    NetworkChunkPublisher *publisher = nullptr;
    ChunkViewSource *source = nullptr;
    ParkedChunks parked;
    std::unordered_set<ChunkPositionAndDimension> sendable;
    std::size_t loaded_cursor = 0;
};

enum class QueueHashCheck {
    Pending,
    Passed,
    Failed,
};

QueueHashCheck queue_hash_check = QueueHashCheck::Pending;
std::size_t queue_hash_checked_keys = 0;
Bedrock::PubSub::Subscription on_chunk_loaded;
std::vector<ChunkPos> loaded_chunks;
std::size_t loaded_chunks_begin = 0;
std::size_t loaded_chunks_seen = 0;
std::uint64_t current_tick_id = 0;

QueuedChunkState getQueuedChunkState(ChunkSource &source, const ChunkPositionAndDimension &key)
{
    const auto chunk = source.getExistingChunk(key.pos);
    if (!chunk) {
        return QueuedChunkState::OutOfView;
    }
    return chunk->getState() == ChunkState::Loaded ? QueuedChunkState::Sendable : QueuedChunkState::NotLoaded;
}

bool checkQueueHash(const ChunkQueue &queue)
{
    std::size_t count = 0;
    for (auto it = queue.begin(); it != queue.end(); ++it, ++count) {
        if (auto found = queue.find(it->first); found == queue.end() || &*found != &*it) {
            return false;
        }
    }
    queue_hash_checked_keys += count;
    return count == queue.size();
}

ParkedChunks::iterator unpark(ParkedChunksComponent &component, ChunkQueue &queue, ParkedChunks::iterator it)
{
    if (auto live = queue.find(it->first); live != queue.end()) {
        if (!it->second.node.mapped().expired()) {
            live->second = it->second.node.mapped();
        }
    }
    else {
        queue.insert(std::move(it->second.node));
    }
    return component.parked.erase(it);
}

ParkedChunks::iterator recheck(ParkedChunksComponent &component, ChunkQueue &queue, ChunkSource &source,
                               ParkedChunks::iterator it)
{
    if (it->second.node.mapped().expired()) {
        return component.parked.erase(it);
    }
    if (queue.contains(it->first)) {
        return unpark(component, queue, it);
    }
    const auto state = getQueuedChunkState(source, it->first);
    if (state == QueuedChunkState::Sendable) {
        return unpark(component, queue, it);
    }
    it->second.state = state;
    return std::next(it);
}

void recheckAll(ParkedChunksComponent &component, ChunkQueue &queue, ChunkSource &source)
{
    for (auto it = component.parked.begin(); it != component.parked.end();) {
        it = recheck(component, queue, source, it);
    }
}
}  // namespace

void ServerPlayer::_updateChunkPublisherView(const Vec3 &position, float min_distance)
{
    // #blameMojang - every tick, each player sorts and walks its whole chunk queue and asks again about every chunk
    // that is still generating. A hundred players flying into new terrain spend half the server tick on it.
    // Fix: hold those chunks out of the queue until they load or the player's view moves.
    auto *publisher = chunk_publisher_view_.get();
    if (!publisher || queue_hash_check == QueueHashCheck::Failed) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::_updateChunkPublisherView, this, position, min_distance);
        return;
    }
    const auto previous_center = ChunkPos(publisher->last_chunk_update_position_);
    const auto previous_radius = publisher->last_chunk_update_radius_;
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::_updateChunkPublisherView, this, position, min_distance);
    auto *source = publisher->source_.get();
    if (!source) {
        return;
    }

    auto &queue = publisher->queued_chunks_;
    if (queue_hash_check == QueueHashCheck::Pending) {
        if (queue.size() < 64) {
            return;
        }
        if (!checkQueueHash(queue)) {
            queue_hash_check = QueueHashCheck::Failed;
            endstone::core::EndstoneServer::getInstance().getLogger().warning(
                "Chunk queue hash does not match the server's, chunks will be sent the slow way.");
            return;
        }
        if (queue_hash_checked_keys < 256) {
            return;
        }
        queue_hash_check = QueueHashCheck::Passed;
        on_chunk_loaded = getLevel().getLevelChunkEventManager()->getOnChunkLoadedConnector().connect(
            [](ChunkSource &, LevelChunk &lc, int) { loaded_chunks.push_back(lc.getPosition()); },
            Bedrock::PubSub::ConnectPosition::AtBack, nullptr);
    }

    if (const auto tick_id = getLevel().getCurrentServerTick().tick_id; tick_id != current_tick_id) {
        current_tick_id = tick_id;
        loaded_chunks.erase(loaded_chunks.begin(),
                            loaded_chunks.begin() + static_cast<std::ptrdiff_t>(loaded_chunks_seen - loaded_chunks_begin));
        loaded_chunks_begin = loaded_chunks_seen;
        loaded_chunks_seen = loaded_chunks_begin + loaded_chunks.size();
    }

    auto &component = getEntity().getOrAddComponent<ParkedChunksComponent>();
    if (component.publisher != publisher) {
        component = ParkedChunksComponent{};
        component.publisher = publisher;
        component.loaded_cursor = loaded_chunks_begin + loaded_chunks.size();
    }
    if (component.source != source || previous_radius == 0) {
        for (auto it = component.parked.begin(); it != component.parked.end();) {
            it = unpark(component, queue, it);
        }
        component.sendable.clear();
    }
    else if (ChunkPos(publisher->last_chunk_update_position_).packed != previous_center.packed ||
             publisher->last_chunk_update_radius_ != previous_radius) {
        component.sendable.clear();
        recheckAll(component, queue, *source);
    }
    component.source = source;

    const auto dimension = getDimensionId();
    if (component.loaded_cursor < loaded_chunks_begin) {
        recheckAll(component, queue, *source);
    }
    else {
        for (auto i = component.loaded_cursor - loaded_chunks_begin; i < loaded_chunks.size(); ++i) {
            auto it = component.parked.find({loaded_chunks[i], dimension});
            if (it != component.parked.end() && it->second.state == QueuedChunkState::NotLoaded) {
                recheck(component, queue, *source, it);
            }
        }
    }
    component.loaded_cursor = loaded_chunks_begin + loaded_chunks.size();

    if ((current_tick_id + (reinterpret_cast<std::uintptr_t>(publisher) >> 4)) % 100 == 0) {
        recheckAll(component, queue, *source);
    }

    std::erase_if(component.sendable, [&](const auto &key) { return !queue.contains(key); });
    for (auto it = queue.begin(); it != queue.end();) {
        if (component.sendable.contains(it->first) || it->second.expired() || it->first.type != dimension) {
            ++it;
            continue;
        }
        const auto key = it->first;
        const auto state = getQueuedChunkState(*source, key);
        if (state == QueuedChunkState::Sendable) {
            component.sendable.insert(key);
            ++it;
            continue;
        }
        auto node = queue.extract(it++);
        auto [parked, inserted] = component.parked.try_emplace(key);
        if (inserted || parked->second.node.mapped().expired()) {
            parked->second = ParkedChunk{std::move(node), state};
        }
    }
}

void ServerPlayer::changeDimension(DimensionType to_id)
{
    auto to_dimension = getLevel().getOrCreateDimension(to_id);
    if (!to_dimension.isSet()) {
        return;
    }

    auto &server = endstone::core::EndstoneServer::getInstance();
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    ChangeDimensionRequest request(getDimensionId(), to_id, getPosition(), Vec3::ZERO, true, false);
    static_cast<IPlayerDimensionTransferer &>(
        getLevel().getPlayerDimensionTransferManager()->getPlayerDimensionTransferConnector())
        .setTransitionLocation(*this, request, *to_dimension.unwrap());

    const auto from_location =
        endstone::core::EndstoneLocation::toEndstone(request.from_position, request.from_dimension);
    const auto to_location = endstone::core::EndstoneLocation::toEndstone(request.to_position, request.to_dimension);
    endstone::PlayerPortalEvent e(player, from_location, to_location);
    server.getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        return;
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::changeDimension, this, to_id);
    // request.to_position = Vec3::ZERO;
    // _setDimensionTransitionComponent(getDimensionId(), to_id, 300);
    // getLevel().requestPlayerChangeDimension(*this, std::move(request));
}
