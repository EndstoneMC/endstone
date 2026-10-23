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

#include "bedrock/world/level/player_location_sender.h"

#include <cmath>
#include <numbers>

bool PlayerLocationSender::_shouldSendPositionPacket(const Vec3 &viewing_player_position,
                                                     const DimensionType &viewing_player_dimension,
                                                     bool viewing_player_is_spectator,
                                                     const OptionalPosition &observed_player_pos_prev,
                                                     const PlayerLocationData &observed_player_position_new) const
{
    if (!observed_player_position_new.position.has_value()) {
        return false;
    }
    if (!viewing_player_is_spectator && observed_player_position_new.is_spectator) {
        return false;
    }
    if (viewing_player_dimension != observed_player_position_new.dimension) {
        return false;
    }

    const Vec3 &observed_position = *observed_player_position_new.position;
    const Vec3 to_observed = observed_position - viewing_player_position;
    const float distance = std::sqrt(to_observed.lengthSquared());
    if (distance < simulation_distance_) {
        return false;
    }
    if (!observed_player_pos_prev.has_value()) {
        return true;
    }

    const Vec3 &previous_position = *observed_player_pos_prev;
    if (distance < 432.0f && (observed_position - previous_position).lengthSquared() > 2304.0f) {
        return true;
    }

    const auto normalized = [](const Vec3 &v, float length) {
        if (length < 0.0001f) {
            return Vec3{0.0f, 0.0f, 0.0f};
        }
        return Vec3{v.x / length, v.y / length, v.z / length};
    };
    const Vec3 to_previous = previous_position - viewing_player_position;
    const Vec3 a = normalized(to_previous, std::sqrt(to_previous.lengthSquared()));
    const Vec3 b = normalized(to_observed, distance);
    const Vec3 cross{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    const float dot = a.x * b.x + a.y * b.y + a.z * b.z;

    constexpr float pi = std::numbers::pi_v<float>;
    float angle = std::fmod(std::atan2(std::sqrt(cross.lengthSquared()), dot) + pi, 2.0f * pi);
    if (angle < 0.0f) {
        angle += 2.0f * pi;
    }
    return angle - pi > 0.06981317f;
}
