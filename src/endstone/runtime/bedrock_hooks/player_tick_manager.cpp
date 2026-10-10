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

namespace {
thread_local bool processing_player_networking = false;
std::vector<ChunkBuildOrderPolicy *> pending_influence_updates;
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
    if (!processing_player_networking) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&ChunkBuildOrderPolicy::updateInfluences, this);
        return;
    }
    if (std::ranges::find(pending_influence_updates, this) == pending_influence_updates.end()) {
        pending_influence_updates.push_back(this);
    }
}
