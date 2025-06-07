

# File map\_canvas.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**map**](dir_35fd4abc90217931459f3a8776f2bf4e.md) **>** [**map\_canvas.h**](map__canvas_8h.md)

[Go to the documentation of this file](map__canvas_8h.md)


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

#include "endstone/util/color.h"
#include "endstone/util/image.h"

namespace endstone {

class MapView;

class MapCanvas {
public:
    virtual ~MapCanvas() = default;

    [[nodiscard]] virtual MapView &getMapView() const = 0;

    virtual void setPixelColor(int x, int y, std::optional<Color> color) = 0;

    [[nodiscard]] virtual std::optional<Color> getPixelColor(int x, int y) const = 0;

    [[nodiscard]] virtual Color getBasePixelColor(int x, int y) const = 0;

    virtual void setPixel(int x, int y, int color) = 0;

    [[nodiscard]] virtual int getPixel(int x, int y) const = 0;

    [[nodiscard]] virtual int getBasePixel(int x, int y) const = 0;

    virtual void drawImage(int x, int y, const Image &image) = 0;

    // TODO(map): getCursors and setCursors
};

}  // namespace endstone
```


