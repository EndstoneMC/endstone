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

#include "bedrock/world/actor/player/abilities.h"
#include "bedrock/world/actor/player/player.h"

namespace PlayerCapabilities {

struct IPlayerData {
    virtual ~IPlayerData() = default;
    [[nodiscard]] virtual bool isAbilityEnabled(AbilitiesIndex) const = 0;
    [[nodiscard]] virtual bool isTeacher() const = 0;
    [[nodiscard]] virtual bool isOperator() const = 0;
};

class PlayerData : public IPlayerData {
public:
    explicit PlayerData(const Player &player);
    bool isAbilityEnabled(AbilitiesIndex) const override;
    bool isTeacher() const override;
    bool isOperator() const override;

private:
    Player const &player_;
};

struct ISharedController {
    virtual ~ISharedController() = 0;
    virtual bool canChat(IPlayerData const &) const = 0;
    virtual bool canTell(IPlayerData const &) const = 0;
    virtual bool canOpenChat(IPlayerData const &) const = 0;
};

}  // namespace PlayerCapabilities
