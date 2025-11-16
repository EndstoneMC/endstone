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

namespace endstone::core {
class EndstoneLocation {
public:
    static Location toEndstone(const Vec3 &vec3, const ::Dimension &dimension)
    {
        return toEndstone(vec3, dimension.getDimensionId());
    }

    static Location toEndstone(const Vec3 &vec3, const DimensionType id)
    {
        return Location(*EndstoneServer::getInstance().getEndstoneLevel()->getDimension(id.runtime_id), vec3.x, vec3.y,
                        vec3.z);
    }
};
}  // namespace endstone::core
