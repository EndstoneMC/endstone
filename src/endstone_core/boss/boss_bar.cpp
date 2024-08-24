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

#include "endstone/detail/boss/boss_bar.h"

namespace endstone::detail {

std::string EndstoneBossBar::getTitle() const
{
    return title_;
}

BarColor EndstoneBossBar::getColor() const
{
    return color_;
}

BarStyle EndstoneBossBar::getStyle() const
{
    return style_;
}

float EndstoneBossBar::getProgress() const
{
    return progress_;
}

bool EndstoneBossBar::isVisible() const
{
    return visible_;
}

bool EndstoneBossBar::shouldDarkenScreen() const
{
    return darken_screen_;
}

std::vector<Player *> EndstoneBossBar::getPlayers() const
{
    return players_;
}

}  // namespace endstone::detail
