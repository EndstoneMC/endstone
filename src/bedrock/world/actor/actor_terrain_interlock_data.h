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

#include <chrono>

class ActorTerrainInterlockData {
    enum class VisibilityState : std::uint8_t {
        InitialNotVisible = 0,
        Visible = 1,
        DelayedDestructionNotVisible = 2,
    };

public:
    ActorTerrainInterlockData();

protected:
    VisibilityState render_visibility_state_;
    std::chrono::steady_clock::time_point creation_time_;
    bool has_been_delayed_deleted_;
};
static_assert(sizeof(ActorTerrainInterlockData) == 24);
