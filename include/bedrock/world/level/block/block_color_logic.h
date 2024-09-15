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

enum BlockColorLogic : std::int32_t {
    Default = 0,
    CoralFan = 0x1,
    DoubleVegetation = 0x2,
    Grass = 0x3,
    Kelp = 0x4,
    Leaves = 0x5,
    Liquid = 0x6,
    RedStoneWire = 0x7,
    Seagrass = 0x8,
    SeaPickle = 0x9,
    ShulkerBox = 0xa,
    Stem = 0xb,
    SugarCane = 0xc,
    TallGrass = 0xd,
    UndyedShulkerBox = 0xe,
    Vine = 0xf,
    Waterlily = 0x10,
};
