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

void ServerWaypoint::update(const Player &viewing_player)
{
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
}
