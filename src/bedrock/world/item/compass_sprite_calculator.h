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

class CompassSpriteCalculator {
public:
    using PointToTargetFunction = BlockPos (*)(const BlockSource *, Actor *);
    using IsPointingAtTargetFunction = bool (*)(const BlockSource *, Actor *);

    explicit CompassSpriteCalculator(PointToTargetFunction, IsPointingAtTargetFunction, float, float);

private:
    PointToTargetFunction target_to_point_to_;
    IsPointingAtTargetFunction is_pointing_at_target_;
    int frame_;
    float rot_;
    float rot_a_;
};
static_assert(sizeof(CompassSpriteCalculator) == 32);
