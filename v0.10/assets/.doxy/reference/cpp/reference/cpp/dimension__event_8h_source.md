

# File dimension\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**level**](dir_9972879ee1a1ab85aef1f41c76fb712c.md) **>** [**dimension\_event.h**](dimension__event_8h.md)

[Go to the documentation of this file](dimension__event_8h.md)


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

#include "endstone/event/level/level_event.h"
#include "endstone/level/dimension.h"

namespace endstone {

class DimensionEvent : public LevelEvent {
public:
    explicit DimensionEvent(Dimension &dimension) : LevelEvent(dimension.getLevel()), dimension_(dimension){};

    [[nodiscard]] Dimension &getDimension() const
    {
        return dimension_;
    }

private:
    Dimension &dimension_;
};

}  // namespace endstone
```


