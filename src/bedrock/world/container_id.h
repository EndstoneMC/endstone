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

enum ContainerID : signed char {
    CONTAINER_ID_NONE = -1,
    CONTAINER_ID_INVENTORY = 0,
    CONTAINER_ID_FIRST = 1,
    CONTAINER_ID_LAST = 100,
    CONTAINER_ID_OFFHAND = 119,
    CONTAINER_ID_ARMOR = 120,
    CONTAINER_ID_SELECTION_SLOTS = 122,
    CONTAINER_ID_PLAYER_ONLY_UI = 124,
    CONTAINER_ID_REGISTRY = 125,
};
