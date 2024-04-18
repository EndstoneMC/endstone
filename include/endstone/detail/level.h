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

#include <unordered_map>

#include "bedrock/world/level/dimension/dimension.h"
#include "bedrock/world/level/level.h"
#include "endstone/actor/actor.h"
#include "endstone/detail/server.h"
#include "endstone/level.h"

using BedrockLevel = ::Level;
using BedrockDimension = ::Dimension;

namespace endstone::detail {

class EndstoneLevel : public endstone::Level {
public:
    explicit EndstoneLevel(BedrockLevel &level, BedrockDimension &dimension);
    ~EndstoneLevel() override = default;

    [[nodiscard]] std::string getName() const override;

    [[nodiscard]] std::vector<Actor *> getActors() const override;

    [[nodiscard]] int getTime() const override;
    void setTime(int time) override;

    [[nodiscard]] BedrockLevel &getBedrockLevel() const;

private:
    EndstoneServer &server_;
    BedrockLevel &level_;
    BedrockDimension &dimension_;
};

}  // namespace endstone::detail
