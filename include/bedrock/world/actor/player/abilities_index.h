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

enum class AbilitiesIndex : std::int16_t {
    Invalid = -1,
    Build = 0,
    Mine = 1,
    DoorsAndSwitches = 2,
    OpenContainers = 3,
    AttackPlayers = 4,
    AttackMobs = 5,
    OperatorCommands = 6,
    Teleport = 7,
    Invulnerable = 8,
    Flying = 9,
    MayFly = 10,
    Instabuild = 11,
    Lightning = 12,
    FlySpeed = 13,
    WalkSpeed = 14,
    Muted = 15,
    WorldBuilder = 16,
    NoClip = 17,
    PrivilegedBuilder = 18,
    AbilityCount = 19,
};
