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

struct DepenetrationComponent {
    enum class Bit : int {
        IsAlwaysOneWayCollision = 0,
        WasPenetratingLastFrame = 1,
        IsStuckInCollider = 2,
        IsStuckItem = 3,
        PushTowardsClosestSpace = 4,
        Count = 5,
    };
    using Bitset = std::bitset<static_cast<std::underlying_type_t<Bit>>(Bit::Count)>;
    Bitset bits;
    Vec3 min_depenetration;
    std::vector<AABB> one_way_physics_blocks;
    std::optional<Vec3> temporary_override;
};

namespace DepenetrationComponentUtility {
static constexpr Vec3 TEMP_Y_OVERRIDE(0, 0.05, 0);
}  // namespace DepenetrationComponentUtility
