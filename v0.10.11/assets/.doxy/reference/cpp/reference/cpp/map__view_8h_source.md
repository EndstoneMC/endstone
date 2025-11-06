

# File map\_view.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**map**](dir_35fd4abc90217931459f3a8776f2bf4e.md) **>** [**map\_view.h**](map__view_8h.md)

[Go to the documentation of this file](map__view_8h.md)


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

#include "endstone/map/map_renderer.h"

namespace endstone {

class Dimension;

class MapView {
public:
    enum class Scale : uint8_t {
        Closest = 0,
        Close = 1,
        Normal = 2,
        Far = 3,
        Farthest = 4
    };

    virtual ~MapView() = default;

    [[nodiscard]] virtual std::int64_t getId() const = 0;

    [[nodiscard]] virtual bool isVirtual() const = 0;

    [[nodiscard]] virtual Scale getScale() const = 0;

    virtual void setScale(Scale scale) = 0;

    [[nodiscard]] virtual int getCenterX() const = 0;

    [[nodiscard]] virtual int getCenterZ() const = 0;

    virtual void setCenterX(int x) = 0;

    virtual void setCenterZ(int z) = 0;

    [[nodiscard]] virtual Dimension *getDimension() const = 0;

    virtual void setDimension(const Dimension &dimension) = 0;

    [[nodiscard]] virtual std::vector<MapRenderer *> getRenderers() const = 0;

    virtual void addRenderer(std::shared_ptr<MapRenderer> renderer) = 0;

    virtual bool removeRenderer(MapRenderer *renderer) = 0;

    [[nodiscard]] virtual bool isUnlimitedTracking() const = 0;

    virtual void setUnlimitedTracking(bool unlimited) = 0;

    [[nodiscard]] virtual bool isLocked() const = 0;

    virtual void setLocked(bool locked) = 0;
};

}  // namespace endstone
```


