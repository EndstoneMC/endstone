

# File sign.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**sign.h**](sign_8h.md)

[Go to the documentation of this file](sign_8h.md)


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

#include "endstone/block/block_state.h"
#include "endstone/block/sign/sign_side.h"

namespace endstone {

class Sign : public BlockState {
public:
    enum class Side {
        Front = 0,

        Back = 1,
    };

    [[nodiscard]] virtual SignSide &getSide(Side side) const = 0;

    [[nodiscard]] virtual bool isWaxed() const = 0;

    virtual void setWaxed(bool waxed) = 0;
};

}  // namespace endstone
```


