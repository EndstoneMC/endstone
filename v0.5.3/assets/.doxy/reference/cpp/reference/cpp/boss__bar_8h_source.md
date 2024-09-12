

# File boss\_bar.h

[**File List**](files.md) **>** [**boss**](dir_d0a7fd8d5b72659767e2a2651b1ff51c.md) **>** [**boss\_bar.h**](boss__bar_8h.md)

[Go to the documentation of this file](boss__bar_8h.md)


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

#include "endstone/boss/bar_color.h"
#include "endstone/boss/bar_flag.h"
#include "endstone/boss/bar_style.h"

namespace endstone {

class Player;

class BossBar {
public:
    virtual ~BossBar() = default;

    [[nodiscard]] virtual std::string getTitle() const = 0;

    virtual void setTitle(std::string title) = 0;

    [[nodiscard]] virtual BarColor getColor() const = 0;

    virtual void setColor(BarColor color) = 0;

    [[nodiscard]] virtual BarStyle getStyle() const = 0;

    virtual void setStyle(BarStyle style) = 0;

    [[nodiscard]] virtual bool hasFlag(BarFlag flag) const = 0;

    virtual void addFlag(BarFlag flag) = 0;

    virtual void removeFlag(BarFlag flag) = 0;

    [[nodiscard]] virtual float getProgress() const = 0;

    virtual void setProgress(float progress) = 0;

    [[nodiscard]] virtual bool isVisible() const = 0;

    virtual void setVisible(bool visible) = 0;

    virtual void addPlayer(Player &player) = 0;

    virtual void removePlayer(Player &player) = 0;

    virtual void removeAll() = 0;

    [[nodiscard]] virtual std::vector<Player *> getPlayers() const = 0;
};

}  // namespace endstone
```


