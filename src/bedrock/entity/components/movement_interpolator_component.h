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

#pragma once

struct MovementInterpolatorComponent {
    void lerpTo(const Vec3 &, const Vec2 &, int);
    void lerpToRotation(const Vec2 &, int);
    void reset();
    void setHeadYawLerpTarget(float, int);
    bool isActive() const;
    static void startLerpTo(EntityContext &, const Vec3 &, const Vec2 &, int);
    Vec3 pos;
    Vec2 rot;
    float head_yaw;
    int position_steps;
    int rotation_steps;
    int head_yaw_steps;
    bool player_control_server_vehicle;
    static constexpr int MAX_LERP_STEPS = 3;
};
static_assert(sizeof(MovementInterpolatorComponent) == 40);
