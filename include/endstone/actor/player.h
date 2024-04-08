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

#include "bedrock/actor/server_player.h"
#include "endstone/actor/human.h"
#include "endstone/util/uuid.h"

namespace endstone {

class Player : public HumanActor {
public:
    explicit Player(ServerPlayer &player) : player_(player){};
    ~Player() override = default;
    [[nodiscard]] std::string getName() const override = 0;
    [[nodiscard]] virtual endstone::UUID getUniqueId() const = 0;

private:
    ServerPlayer &player_;
};

}  // namespace endstone
