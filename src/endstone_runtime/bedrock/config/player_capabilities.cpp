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

#include "bedrock/config/player_capabilities.h"

namespace PlayerCapabilities {

PlayerData::PlayerData(const Player &player) : player_(player) {}

bool PlayerData::isAbilityEnabled(AbilitiesIndex index) const
{
    return player_.getAbilities().getBool(index);
}

bool PlayerData::isTeacher() const
{
    return player_.isTeacher();
}

bool PlayerData::isOperator() const
{
    return player_.getPlayerPermissionLevel() >= PlayerPermissionLevel::Operator;
}

}  // namespace PlayerCapabilities
