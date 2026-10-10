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
#include <vector>

#include "bedrock/world/level/dimension/chunk_build_order_policy.h"
#include "endstone/runtime/hook.h"

#ifdef ENDSTONE_VERIFY_PERF
#include <chrono>
#include <cstdint>

#include "endstone/core/server.h"
#endif

namespace {
thread_local bool processing_player_networking = false;
std::vector<ChunkBuildOrderPolicy *> pending_influence_updates;
#ifdef ENDSTONE_VERIFY_PERF
std::uint64_t deferred_influence_updates = 0;
std::uint64_t flushed_influence_updates = 0;
auto last_influence_summary = std::chrono::steady_clock::now();
#endif
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
#ifdef ENDSTONE_VERIFY_PERF
    flushed_influence_updates += pending_influence_updates.size();
    if (const auto now = std::chrono::steady_clock::now(); now - last_influence_summary >= std::chrono::seconds(30)) {
        last_influence_summary = now;
        endstone::core::EndstoneServer::getInstance().getLogger().info(
            "[verify] influences: deferred calls {}, flushed calls {}", deferred_influence_updates,
            flushed_influence_updates);
    }
#endif
    pending_influence_updates.clear();
}

void ChunkBuildOrderPolicy::updateInfluences()
{
    if (!processing_player_networking) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::updateInfluences, this);
        return;
    }
#ifdef ENDSTONE_VERIFY_PERF
    ++deferred_influence_updates;
#endif
    if (std::ranges::find(pending_influence_updates, this) == pending_influence_updates.end()) {
        pending_influence_updates.push_back(this);
    }
}
