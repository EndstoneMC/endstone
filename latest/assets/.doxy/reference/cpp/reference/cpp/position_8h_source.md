

# File position.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**level**](dir_8e239ca1e5fd0d936d66a30330d3a329.md) **>** [**position.h**](position_8h.md)

[Go to the documentation of this file](position_8h.md)


```C++
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

#include <memory>
#include <optional>

#include "endstone/util/vector.h"

namespace endstone {

class Level;
class Dimension;

class Position : public Vector<float> {
public:
    Position(Dimension *dimension, float x, float y, float z) : Vector(x, y, z), dimension_(dimension) {}

    [[nodiscard]] Dimension *getDimension() const
    {
        return dimension_;
    }

    void setDimension(Dimension &dimension)
    {
        dimension_ = &dimension;
    }

    [[nodiscard]] int getBlockX() const
    {
        return static_cast<int>(std::floorf(x_));
    }

    [[nodiscard]] int getBlockY() const
    {
        return static_cast<int>(std::floorf(y_));
    }

    [[nodiscard]] int getBlockZ() const
    {
        return static_cast<int>(std::floorf(z_));
    }

private:
    Dimension *dimension_;
};

}  // namespace endstone
```


