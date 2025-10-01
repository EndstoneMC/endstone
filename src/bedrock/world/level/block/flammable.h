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

enum class BurnOdds : uint16_t {
    INSTANT = 100,
    EASY = 60,
    MEDIUM = 20,
    HARD = 5,
    NEVER = 0,
};

enum class FlameOdds : uint16_t {
    INSTANT = 60,
    EASY = 30,
    MEDIUM = 15,
    HARD = 5,
    NEVER = 0,
};

enum class LavaFlammable : bool {
    ALWAYS = 1,
    NEVER = 0,
};
