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

#include "bedrock/world/server_waypoint.h"

#include <cmath>
#include <limits>

#include "bedrock/world/actor/player/player.h"

#ifdef ENDSTONE_VERIFY_PERF
#include <bit>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace {
struct WaypointState {
    std::uint32_t update_flags;
    std::optional<std::string> texture_path;
    Vec2 icon_size;
    bool is_visible;

    bool operator==(const WaypointState &other) const
    {
        return update_flags == other.update_flags && texture_path == other.texture_path &&
               std::bit_cast<std::uint32_t>(icon_size.x) == std::bit_cast<std::uint32_t>(other.icon_size.x) &&
               std::bit_cast<std::uint32_t>(icon_size.y) == std::bit_cast<std::uint32_t>(other.icon_size.y) &&
               is_visible == other.is_visible;
    }
};

struct VerifyStats {
    std::uint64_t checked = 0;
    std::uint64_t divergences = 0;
    std::uint64_t details = 0;
    std::chrono::steady_clock::time_point last_summary = std::chrono::steady_clock::now();
};
VerifyStats verify_stats;

void verifyResult(const WaypointState &ours, const WaypointState &theirs)
{
    auto &stats = verify_stats;
    const auto &logger = endstone::core::EndstoneServer::getInstance().getLogger();
    ++stats.checked;
    if (ours != theirs) {
        ++stats.divergences;
        if (stats.details++ < 50) {
            logger.warning("[verify] locator waypoint update divergence: flags {:#x}/{:#x} visible {}/{} texture "
                           "'{}'/'{}' icon {},{}/{},{}",
                           ours.update_flags, theirs.update_flags, ours.is_visible, theirs.is_visible,
                           ours.texture_path.value_or("<none>"), theirs.texture_path.value_or("<none>"),
                           ours.icon_size.x, ours.icon_size.y, theirs.icon_size.x, theirs.icon_size.y);
        }
    }
    if (const auto now = std::chrono::steady_clock::now(); now - stats.last_summary >= std::chrono::seconds(30)) {
        stats.last_summary = now;
        logger.info("[verify] locator waypoint update: checked={} divergences={}", stats.checked, stats.divergences);
    }
}
}  // namespace
#endif

void ServerWaypoint::update(const Player &viewing_player)
{
#ifdef ENDSTONE_VERIFY_PERF
    const auto before = WaypointState{update_flags_, texture_path_, icon_size_, is_visible_};
#endif
    // #blameMojang - vanilla copies the selected texture path twice per call, once per waypoint per player per tick.
    const Texture *selected = nullptr;
    if (!texture_selector_.textures.empty()) {
        const auto &viewer = viewing_player.getPosition();
        const auto dx = world_pos_.pos.x - viewer.x;
        const auto dy = world_pos_.pos.y - viewer.y;
        const auto dz = world_pos_.pos.z - viewer.z;
        const auto distance = std::sqrt(dz * dz + (dy * dy + dx * dx));
        for (const auto &texture : texture_selector_.textures) {
            if (texture.upper_bound && !(static_cast<float>(*texture.upper_bound) > distance)) {
                continue;
            }
            if (distance >= static_cast<float>(texture.lower_bound)) {
                selected = &texture;
                break;
            }
        }
    }

    if (!selected) {
        if (texture_path_) {
            setTexturePath(std::nullopt);
        }
    }
    else {
        if (texture_path_ != selected->texture_path) {
            setTexturePath(selected->texture_path);
        }
        constexpr auto epsilon = std::numeric_limits<float>::epsilon();
        if (!(epsilon >= std::fabs(selected->icon_size.x - icon_size_.x)) ||
            !(epsilon >= std::fabs(selected->icon_size.y - icon_size_.y))) {
            update_flags_ |= static_cast<std::uint32_t>(UpdateFlags::IconSize);
            icon_size_ = selected->icon_size;
        }
    }

    if (const auto is_visible = calculateIsVisible(viewing_player); is_visible_ != is_visible) {
        update_flags_ |= static_cast<std::uint32_t>(UpdateFlags::Visibility);
        is_visible_ = is_visible;
    }
#ifdef ENDSTONE_VERIFY_PERF
    const auto ours = WaypointState{update_flags_, texture_path_, icon_size_, is_visible_};
    update_flags_ = before.update_flags;
    texture_path_ = before.texture_path;
    icon_size_ = before.icon_size;
    is_visible_ = before.is_visible;
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerWaypoint::update, this, viewing_player);
    verifyResult(ours, WaypointState{update_flags_, texture_path_, icon_size_, is_visible_});
#endif
}
