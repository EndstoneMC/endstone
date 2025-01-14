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

#include "endstone/core/game_mode.h"

#include "endstone/detail/utility.h"

namespace endstone::core {

GameMode EndstoneGameMode::fromMinecraft(GameType type)
{
    switch (type) {
    case GameType::Creative:
        return GameMode::Creative;
    case GameType::Adventure:
        return GameMode::Adventure;
    case GameType::Spectator:
        return GameMode::Spectator;
    default:
        break;
    }
    return GameMode::Survival;
}

GameType EndstoneGameMode::toMinecraft(GameMode mode)
{
    switch (mode) {
    case GameMode::Survival:
        return GameType::Survival;
    case GameMode::Creative:
        return GameType::Creative;
    case GameMode::Adventure:
        return GameType::Adventure;
    case GameMode::Spectator:
        return GameType::Spectator;
    default:
        detail::unreachable();
    }
}

}  // namespace endstone::core
