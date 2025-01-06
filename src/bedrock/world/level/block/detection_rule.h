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

#include <cstdint>

enum LiquidType : std::uint8_t {
    WATER = 0,
};

enum LiquidReaction : std::uint8_t {
    BROKEN = 0,
    POPPED = 1,
    BLOCKING = 2,
    NOREACTION = 3,
};

struct BlockedDirections {
    std::uint8_t stops_flow_directions_mask;
};

struct DetectionRule {
    bool can_contain_liquid;
    BlockedDirections stop_flow_directions;
    LiquidReaction on_liquid_touches;
    LiquidType liquid_type;
};
