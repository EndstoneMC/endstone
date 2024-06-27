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

enum class BlockProperty : std::uint64_t {
    None = 0,
    Stair = 1LL << 0,
    HalfSlab = 1LL << 1,
    Hopper = 1LL << 2,
    TopSnow = 1LL << 3,
    FenceGate = 1LL << 4,
    Leaf = 1LL << 5,
    ThinConnects2D = 1LL << 6,
    Connects2D = 1LL << 7,
    Carpet = 1LL << 8,
    Button = 1LL << 9,
    Door = 1LL << 10,
    Portal = 1LL << 11,
    Heavy = 1LL << 12,
    Snow = 1LL << 13,
    Trap = 1LL << 14,
    Sign = 1LL << 15,
    Walkable = 1LL << 16,
    PressurePlate = 1LL << 17,
    PistonBlockGrabber = 1LL << 18,
    TopSolidBlocking = 1LL << 19,
    SolidBlocking = 1LL << 20,
    CubeShaped = 1LL << 21,
    Power_NO = 1LL << 22,
    Power_BlockDown = 1LL << 23,
    Immovable = 1LL << 24,
    BreakOnPush = 1LL << 25,
    Piston = 1LL << 26,
    InfiniBurn = 1LL << 27,
    RequiresWorldBuilder = 1LL << 28,
    CausesDamage = 1LL << 29,
    BreaksWhenFallenOnByHeavy = 1LL << 30,
    OnlyPistonPush = 1LL << 31,
    Liquid = 1LL << 32,
    CanBeBuiltOver = 1LL << 33,
    SnowRecoverable = 1LL << 34,
    Scaffolding = 1LL << 35,
    CanSupportCenterHangingBlock = 1LL << 36,
    BreaksWhenHitByArrow = 1LL << 37,
    Unwalkable = 1LL << 38,
    // Property_39 = 1LL << 39,
    Hollow = 1LL << 40,
    OperatorBlock = 1LL << 41,
    SupportedByFlowerPot = 1LL << 42,
    PreventsJumping = 1LL << 43,
    ContainsHoney = 1LL << 44,
    Slime = 1LL << 45,
    SculkReplaceable = 1LL << 46,
    Climbable = 1LL << 47,
    CanHaltWhenClimbing = 1LL << 48,
    _entt_enum_as_bitmask
};
