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

enum class ClientInputLockCategory :std:: uint8_t {
    Undefined = 0,
    Camera = 1,
    Movement = 2,
    Emoting = 3,
    LateralMovement = 4,
    Sneak = 5,
    Jump = 6,
    Mount = 7,
    Dismount = 8,
    MoveForward = 9,
    MoveBackward = 10,
    MoveLeft = 11,
    MoveRight = 12,
    _Count = 13,
};
