

# File rotation.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**rotation.h**](rotation_8h.md)

[Go to the documentation of this file](rotation_8h.md)


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

namespace endstone {

enum class Rotation {
    None = 0,

    Clockwise45 = 1,

    Clockwise = 2,

    Clockwise135 = 3,

    Flipped = 4,

    Flipped45 = 5,

    CounterClockwise = 6,

    CounterClockwise45 = 7,
};

}  // namespace endstone
```


