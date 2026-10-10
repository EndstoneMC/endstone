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

#include "bedrock/world/actor/player/vanilla_waypoint_manager.h"

#include <algorithm>
#include <cstdint>
#include <vector>

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/actor/player/player_list_entry.h"
#include "bedrock/world/level/level.h"
#include "endstone/runtime/hook.h"

#ifdef ENDSTONE_VERIFY_PERF
#include <chrono>
#include <cstddef>

#include "endstone/core/server.h"

namespace {
struct VerifyStats {
    std::uint64_t checked = 0;
    std::uint64_t skipped = 0;
    std::uint64_t grew_after_skip = 0;
    std::uint64_t conservative = 0;
    std::uint64_t details = 0;
    std::chrono::steady_clock::time_point last_summary = std::chrono::steady_clock::now();
};
VerifyStats verify_stats;

void verifyResult(bool skipped, std::size_t before, std::size_t after, ActorUniqueID player)
{
    auto &stats = verify_stats;
    const auto &logger = endstone::core::EndstoneServer::getInstance().getLogger();
    ++stats.checked;
    if (skipped) {
        ++stats.skipped;
        if (after != before) {
            ++stats.grew_after_skip;
            if (stats.details++ < 50) {
                logger.warning("[verify] locator waypoints divergence: player {} group grew {} -> {} after a skip",
                               player.raw_id, before, after);
            }
        }
    }
    else if (after == before) {
        ++stats.conservative;
    }
    if (const auto now = std::chrono::steady_clock::now(); now - stats.last_summary >= std::chrono::seconds(30)) {
        stats.last_summary = now;
        logger.info("[verify] locator waypoints: checked={} skipped={} grew_after_skip={} conservative={}",
                    stats.checked, stats.skipped, stats.grew_after_skip, stats.conservative);
    }
}
}  // namespace
#endif

void VanillaWaypointManager::update(Player &self, ServerLocatorBar &server_locator_bar, bool is_locator_bar_enabled)
{
    // #blameMojang - vanilla scans the whole group for every player-list entry, every player, every tick.
    if (is_locator_bar_enabled_ == is_locator_bar_enabled && waypoint_group_) {
        std::vector<std::int64_t> ids;
        ids.reserve(waypoint_group_->waypoints_.size());
        for (const auto &[handle, waypoint] : waypoint_group_->waypoints_) {
            if (!waypoint) {
                continue;
            }
            if (const auto id = waypoint->tryGetActorID()) {
                ids.push_back(id->raw_id);
            }
        }
        std::ranges::sort(ids);
        const auto self_id = self.getOrCreateUniqueID();
        const auto &level = self.getLevel();
        const auto missing = std::ranges::any_of(level.getPlayerList(), [&](const auto &entry) {
            const auto id = entry.second.id;
            return id != self_id && !std::ranges::binary_search(ids, id.raw_id) && level.fetchEntity(id, false);
        });
#ifdef ENDSTONE_VERIFY_PERF
        const auto before = waypoint_group_->waypoints_.size();
        ENDSTONE_HOOK_CALL_ORIGINAL(&VanillaWaypointManager::update, this, self, server_locator_bar,
                                    is_locator_bar_enabled);
        verifyResult(!missing, before, waypoint_group_ ? waypoint_group_->waypoints_.size() : 0, self_id);
        return;
#endif
        if (!missing) {
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&VanillaWaypointManager::update, this, self, server_locator_bar,
                                is_locator_bar_enabled);
}
