

# File sign\_side.h

[**File List**](files.md) **>** [**block**](dir_faca67fc60a7463eb1bd30eabe023cf1.md) **>** [**sign**](dir_a49b0d7cd186ac75e821684211f814e0.md) **>** [**sign\_side.h**](sign__side_8h.md)

[Go to the documentation of this file](sign__side_8h.md)


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

#include <string>
#include <vector>

#include "endstone/util/color.h"

namespace endstone {

class SignSide {
public:
    virtual ~SignSide() = default;

    [[nodiscard]] virtual std::vector<std::string> getLines() const = 0;

    [[nodiscard]] virtual std::string getLine(int index) const = 0;

    virtual void setLine(int index, std::string line) = 0;

    [[nodiscard]] virtual bool isGlowingText() const = 0;

    virtual void setGlowingText(bool glowing) = 0;

    [[nodiscard]] virtual Color getColor() const = 0;

    virtual void setColor(Color color) = 0;
};

}  // namespace endstone
```


