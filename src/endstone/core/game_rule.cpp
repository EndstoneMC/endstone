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

#include "endstone/core/game_rule.h"

#include <algorithm>
#include <cctype>

namespace endstone::core {

EndstoneGameRule::EndstoneGameRule(const ::GameRule &game_rule) : game_rule_(game_rule), key_(game_rule.getName())
{
    std::ranges::transform(key_, key_.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}

Identifier<GameRule> EndstoneGameRule::getId() const
{
    return {Identifier<GameRule>::Minecraft, key_};
}

std::string EndstoneGameRule::getTranslationKey() const
{
    throw std::runtime_error("Bedrock does not localize game rule names, so a game rule has no translation key.");
}

const ::GameRule &EndstoneGameRule::getHandle() const
{
    return game_rule_;
}

}  // namespace endstone::core
