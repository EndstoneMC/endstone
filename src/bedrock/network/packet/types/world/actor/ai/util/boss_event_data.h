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

enum class BossEventUpdateType :int{
    Add = 0,
    PlayerAdded = 1,
    Remove = 2,
    PlayerRemoved = 3,
    UpdatePercent = 4,
    UpdateName = 5,
    UpdateProperties = 6,
    UpdateStyle = 7,
    Query = 8,
};

enum class BossBarColor :int {
    PINK = 0,
    BLUE = 1,
    RED = 2,
    GREEN = 3,
    YELLOW = 4,
    PURPLE = 5,
    REBECCA_PURPLE = 6,
    WHITE = 7,
};

enum class BossBarOverlay :int {
    PROGRESS = 0,
    NOTCHED_6 = 1,
    NOTCHED_10 = 2,
    NOTCHED_12 = 3,
    NOTCHED_20 = 4,
};