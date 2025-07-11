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

namespace persona {
class ArmSize {
public:
    enum class Type : size_t {
        Slim = 0,
        Wide = 1,
        Count = 2,
        Unknown = 3,
    };
};

enum class AnimatedTextureType : unsigned int {
    None = 0,
    Face = 1,
    Body32x32 = 2,
    Body128x128 = 3,
};

enum class AnimationExpression : unsigned int {
    Linear = 0,
    Blinking = 1,
};

enum class PieceType : unsigned int;
}  // namespace persona
