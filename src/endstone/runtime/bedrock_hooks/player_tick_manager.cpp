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

#include "bedrock/world/level/player_tick_manager.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "bedrock/world/level/dimension/chunk_build_order_policy.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace endstone::runtime {
// One policy's influences as of its last updateInfluences.
struct ChunkBuildOrderSnapshot {
    std::uint64_t version = 0;
    std::vector<ChunkBuildOrderPolicy::Influence> others;
    std::vector<ChunkBuildOrderPolicy::Influence> players;
    std::vector<std::uint32_t> xs;
    std::vector<std::uint32_t> zs;
    std::vector<double> look_x;
    std::vector<double> look_z;
    int min_x = 0;
    int max_x = 0;
    int min_z = 0;
    int max_z = 0;
};
}  // namespace endstone::runtime

namespace {
thread_local bool processing_player_networking = false;
std::vector<ChunkBuildOrderPolicy *> pending_influence_updates;

// Per-policy snapshot; version is odd while updateInfluences is swapping, even once the snapshot matches the vector.
struct InfluenceSnapshotSlot {
    std::atomic<const ChunkBuildOrderPolicy *> policy;
    std::atomic<std::uint64_t> version;
    std::mutex mutex;
    std::shared_ptr<const endstone::runtime::ChunkBuildOrderSnapshot> snapshot;
};
std::array<InfluenceSnapshotSlot, 8> influence_snapshots;
std::atomic<bool> influence_snapshots_disabled{false};
}  // namespace

void PlayerTickManager::processPlayerNetworking(const Tick &current_tick)
{
    // #blameMojang - every player's tick rebuilds the chunk generation priority list from every player, then waits
    // for the generation threads to let go of it. With 100 players that is 100 rebuilds per tick.
    // Fix: rebuild it once after all players have ticked.
    processing_player_networking = true;
    ENDSTONE_HOOK_CALL_ORIGINAL(&PlayerTickManager::processPlayerNetworking, this, current_tick);
    processing_player_networking = false;
    for (auto *policy : pending_influence_updates) {
        policy->updateInfluences();
    }
    pending_influence_updates.clear();
}

void ChunkBuildOrderPolicy::updateInfluences()
{
    if (processing_player_networking) {
        if (std::ranges::find(pending_influence_updates, this) == pending_influence_updates.end()) {
            pending_influence_updates.push_back(this);
        }
        return;
    }

    InfluenceSnapshotSlot *slot = nullptr;
    for (auto &candidate : influence_snapshots) {
        const ChunkBuildOrderPolicy *owner = nullptr;
        if (candidate.policy.compare_exchange_strong(owner, this) || owner == this) {
            slot = &candidate;
            break;
        }
    }
    if (!slot || influence_snapshots_disabled.load(std::memory_order_relaxed)) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::updateInfluences, this);
        return;
    }

    // Seqlock: odd from before vanilla's swap until the snapshot of the new vector is published.
    std::lock_guard slot_lock(slot->mutex);
    slot->version.fetch_add(1);
    // Vanilla ChunkBuildOrderPolicy::updateInfluences: copies mPositionMap into a new vector and swaps it in.
    ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::updateInfluences, this);

    // Snapshot the new vector: players with |look y| <= 2 can be pruned, every other influence is always scored.
    auto snapshot = std::make_shared<endstone::runtime::ChunkBuildOrderSnapshot>();
    auto &s = *snapshot;
    const auto map_size = position_map_.size();
    auto layout_matches = false;
    {
        std::lock_guard lock(chunk_pos_async_spin_lock_);
        // Layout check: the vector holds exactly one influence per map entry.
        layout_matches = chunk_pos_and_direction_async_.size() == map_size;
        if (layout_matches) {
            for (const auto &influence : chunk_pos_and_direction_async_) {
                if (influence.type == HintType::Player && std::fabs(influence.parameter.y) <= 2.0F) {
                    s.players.push_back(influence);
                }
                else {
                    s.others.push_back(influence);
                }
            }
        }
    }
    if (!layout_matches) {
        if (!influence_snapshots_disabled.exchange(true)) {
            endstone::core::EndstoneServer::getInstance().getLogger().warning(
                "Unexpected chunk build order policy layout, chunk generation order falls back to vanilla scoring.");
        }
        return;
    }

    // Player positions as vanilla's uint32 operands, their bounding box, and their unit horizontal look.
    if (!s.players.empty()) {
        s.min_x = s.max_x = s.players.front().position.x;
        s.min_z = s.max_z = s.players.front().position.z;
    }
    for (const auto &player : s.players) {
        s.min_x = std::min(s.min_x, player.position.x);
        s.max_x = std::max(s.max_x, player.position.x);
        s.min_z = std::min(s.min_z, player.position.z);
        s.max_z = std::max(s.max_z, player.position.z);
        s.xs.push_back(static_cast<std::uint32_t>(player.position.x));
        s.zs.push_back(static_cast<std::uint32_t>(player.position.z));
        const double look_x = player.parameter.x;
        const double look_z = player.parameter.z;
        const auto length = std::sqrt(look_x * look_x + look_z * look_z);
        const auto facing = length > 0.0 && std::isfinite(length);
        s.look_x.push_back(facing ? look_x / length : std::numeric_limits<double>::quiet_NaN());
        s.look_z.push_back(facing ? look_z / length : 0.0);
    }

    s.version = slot->version.load(std::memory_order_relaxed) + 1;
    slot->snapshot = std::move(snapshot);
    slot->version.fetch_add(1, std::memory_order_release);
}

std::int32_t ChunkBuildOrderPolicy::getChunkRebuildPriority(const ChunkPos &cp) const
{
    // #blameMojang - every chunk generation pass scores each waiting chunk against every player, under a lock.
    // With 100 players the pass takes longer than the chunks it launches, and the generation threads sit idle.
    // Fix: score against this tick's copy of the influences, running vanilla's scorer on only the players that can win.
    // Vanilla ChunkBuildOrderPolicy::getChunkRebuildPriority: min over every influence under mChunkPosAsyncSpinLock.
    std::size_t index = 0;
    while (index < influence_snapshots.size() &&
           influence_snapshots[index].policy.load(std::memory_order_acquire) != this) {
        ++index;
    }
    if (index == influence_snapshots.size() || influence_snapshots_disabled.load(std::memory_order_relaxed)) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, this, cp);
    }

    // Seqlock read: use the cached snapshot only if it carries the slot's current even version.
    auto &slot = influence_snapshots[index];
    const auto version = slot.version.load(std::memory_order_acquire);
    thread_local std::array<std::pair<std::uint64_t, std::shared_ptr<const endstone::runtime::ChunkBuildOrderSnapshot>>,
                            influence_snapshots.size()>
        cached_snapshots;
    auto &[cached_version, snapshot] = cached_snapshots[index];
    if (version % 2 == 0 && cached_version != version) {
        std::lock_guard lock(slot.mutex);
        snapshot = slot.snapshot;
        cached_version = version;
    }
    if (!snapshot || snapshot->version != version) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, this, cp);
    }

    // Few players: vanilla's full loop is cheaper than pruning.
    const auto &players = snapshot->players;
    if (players.size() < 16) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, this, cp);
    }

    // Vanilla wraps dx * dx + dz * dz in int32: leave any chunk where a player could overflow it to vanilla.
    const auto far_x =
        std::max(std::abs(std::int64_t{cp.x} - snapshot->min_x), std::abs(std::int64_t{cp.x} - snapshot->max_x));
    const auto far_z =
        std::max(std::abs(std::int64_t{cp.z} - snapshot->min_z), std::abs(std::int64_t{cp.z} - snapshot->max_z));
    if (far_x > 46340 || far_z > 46340 || far_x * far_x + far_z * far_z > std::numeric_limits<std::int32_t>::max()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, this, cp);
    }

    // A policy holding only the candidates, so vanilla's own scorer computes every score bit for bit.
    struct FakePolicy {
        FakePolicy()
        {
            std::construct_at(&policy->chunk_pos_async_spin_lock_);
            std::construct_at(&policy->chunk_pos_and_direction_async_);
        }
        ~FakePolicy() { std::destroy_at(&policy->chunk_pos_and_direction_async_); }
        alignas(ChunkBuildOrderPolicy) std::byte storage[sizeof(ChunkBuildOrderPolicy)]{};
        ChunkBuildOrderPolicy *policy = reinterpret_cast<ChunkBuildOrderPolicy *>(storage);
    };
    thread_local FakePolicy fake;
    auto &candidates = fake.policy->chunk_pos_and_direction_async_;

    // Squared distance to every player, with vanilla's integer math, and the nearest player.
    const auto count = players.size();
    const auto qx = static_cast<std::uint32_t>(cp.x);
    const auto qz = static_cast<std::uint32_t>(cp.z);
    thread_local std::vector<std::uint32_t> distances;
    thread_local std::vector<std::uint32_t> survivors;
    distances.resize(count);
    survivors.resize(count);
    auto *distance = distances.data();
    auto *survivor = survivors.data();
    const auto *xs = snapshot->xs.data();
    const auto *zs = snapshot->zs.data();
    auto nearest_distance = std::numeric_limits<std::uint32_t>::max();
    for (std::size_t k = 0; k < count; ++k) {
        distance[k] = (qx - xs[k]) * (qx - xs[k]) + (qz - zs[k]) * (qz - zs[k]);
        nearest_distance = std::min(nearest_distance, distance[k]);
    }
    const auto nearest = static_cast<std::size_t>(std::find(distance, distance + count, nearest_distance) - distance);

    // Same scorer on the nearest player alone gives its exact score.
    candidates.assign(1, players[nearest]);
    const auto nearest_score =
        ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, fake.policy, cp);

    // Every player scores above 0.2499 * d - 1, so one with d^2 >= 17 * bound^2 cannot beat the nearest player.
    const std::int64_t bound = std::clamp(nearest_score, 0, 1 << 20) + 1;
    const auto limit = 17 * bound * bound;
    std::size_t survivor_count = 0;
    for (std::size_t k = 0; k < count; ++k) {
        survivor[survivor_count] = static_cast<std::uint32_t>(k);
        survivor_count += distance[k] < limit || k == nearest;
    }

    // The facing factor is at least 1 - 0.75 * cos, so a survivor looking away cannot beat it either.
    std::size_t kept_count = 0;
    for (std::size_t i = 0; i < survivor_count; ++i) {
        const auto k = survivor[i];
        const auto dx = static_cast<double>(static_cast<std::int32_t>(qx - xs[k]));
        const auto dz = static_cast<double>(static_cast<std::int32_t>(qz - zs[k]));
        const auto d = std::sqrt(static_cast<double>(distance[k]));
        const auto toward = std::isnan(snapshot->look_x[k]) ? d : dx * snapshot->look_x[k] + dz * snapshot->look_z[k];
        survivor[kept_count] = k;
        kept_count += k == nearest || (d - 0.75 * std::max(0.0, toward + 1e-5 * d)) * (1.0 - 2e-6) < bound;
    }
    candidates.assign(snapshot->others.begin(), snapshot->others.end());
    for (std::size_t i = 0; i < kept_count; ++i) {
        candidates.push_back(players[survivor[i]]);
    }

    // Same scorer on the other influences and the remaining players; the nearest player alone is already scored.
    const auto result =
        snapshot->others.empty() && candidates.size() == 1
            ? nearest_score
            : ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, fake.policy, cp);

    // Self-check against vanilla: the first 4096 calls per thread, then 1 in 4096; a mismatch disables the snapshots.
    thread_local std::uint64_t calls = 0;
    ++calls;
    if (calls > 4096 && calls % 4096 != 0) {
        return result;
    }
    const auto expected = ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::getChunkRebuildPriority, this, cp);
    if (expected != result && slot.version.load(std::memory_order_acquire) == version &&
        !influence_snapshots_disabled.exchange(true)) {
        endstone::core::EndstoneServer::getInstance().getLogger().warning(
            "Chunk ({}, {}) scored {} instead of {}, chunk generation order falls back to vanilla scoring.", cp.x, cp.z,
            result, expected);
    }
    return expected;
}
